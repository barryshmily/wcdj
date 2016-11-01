#include <cstdlib>// srand and rand  
#include <ctime>// time  
#include "stats_client.h"
#include "ipc_fifo.h"
#include "ncs_config.h" 
#include "scoped_ptr.h"

using tdf::ipc::fifo_delay;
using tdf::ipc::fifo_fail;
using tdf::ipc::fifo;

#define LOG(format, ...) \
	do { \
		if (g_enable_stats_client_log) {\
			char __log_buff__[4096] = {0}; \
			char time[32] = {0}; \
			getstimeval(time); \
			snprintf(__log_buff__, sizeof(__log_buff__), "[%s][%ld][%s][%s:%s:%d]" format"\n", time, getpid(), g_uuid.c_str(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
			FILE* __fp=fopen("./stats_client.log","a+"); \
			fwrite(__log_buff__, strlen(__log_buff__), 1 ,__fp ); \
			fclose(__fp); \
		} \
	} while(0)

#define GET_GCD(m, n, result) do{ \
	result = n; \
	while (result) \
	{ \
		result = m % n; \
		m = n; \
		n = result; \
	} \
	result = m; \
} while(0)

enum E_NODE_STATUS {
	E_NODE_STATUS_OK = 1,  
	E_NODE_STATUS_FAIL,   
	E_NODE_STATUS_PROBE
};

typedef struct node_attr_s
{
public:
	node_attr_s() {}
	node_attr_s(int w, int s) : weight(w), status(s) {}

	int weight;
	int status;

} node_attr_t;

typedef struct service_attr_s
{
public:
	service_attr_s() {}
	service_attr_s(std::string m, std::string v) : mode(m), ver(v) {}

	std::string mode;
	std::string ver;

} service_attr_t;

// <service, <ip:port, node_attr_t> >
typedef std::map<std::string, std::map<std::string, node_attr_t> > stats_shm_conf_type;
// <service, service_attr_t>
typedef std::map<std::string, service_attr_t> stats_shm_conf_service_attr_type;

typedef struct node_s
{
	std::string host;
	int weight;
	node_s(const std::string &h, int w) : host(h), weight(w) {}
} node_t;


// global vars
int g_enable_stats_client_log;
std::string g_uuid;
const std::string ROOT_NODE = "stats_conf";
int g_need_update;
stats_shm_conf_type g_stats_shm_conf_map;
stats_shm_conf_service_attr_type g_stats_shm_conf_service_attr_map;
std::string g_mode;
std::string g_ver;
std::string g_node_selected;
std::vector<node_t> g_ready_nodes;               
std::vector<node_t> g_detect_nodes;
std::vector<node_t> g_sp_retry_nodes;
std::map<int, std::vector<std::string> > g_ready_groups;  // sp used
std::map<int, std::vector<std::string> > g_detect_groups; // sp used
int g_last_index;
int g_current_weight;
int g_max_w;
int g_gcd;
double g_perhaps = 0.05;


static char * getstimeval(char *buf)
{
	struct timeval tv;
	struct tm      tm;
	size_t         len = 28;

	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tm);
	strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm);
	len = strlen(buf);
	sprintf(buf + len, " %06.6d", (int)(tv.tv_usec));

	return buf;
}

static double zero_to_one()// [0,1]  
{  
    return (double)rand() / RAND_MAX;  
}

static int is_maybe_true(double &ratio)
{
	double r = zero_to_one();
	int is_true = (ratio >= r);

	LOG("ratio[%f] >= r[%f] is_true[%d]", ratio, r, is_true);
	return is_true;
}

static bool node_compare(const node_t &left, const node_t &right)
{
	return (left.weight > right.weight);
}

static int proc_init(stats_info_t &stats_info, std::string &err_msg)
{
	int ret = TDF_STATS_OK;

	// note, we can not get FRuleMode from mod_balance.xml on stats process init, because we do not know func
	std::string service = stats_info.service_;
	std::map<std::string, node_attr_t> &node_map = g_stats_shm_conf_map[service];

	int max_w = 0;
	std::map<std::string, node_attr_t>::iterator it = node_map.begin();
	for (; it != node_map.end(); ++it) {
		int w = it->second.weight;
		if (w > max_w) {
			if (it->first == stats_info.disable_node_) {
				//LOG("disable_node[%s]", stats_info.disable_node_);
				continue;
			}
			max_w = w;
			g_node_selected = it->first;// the first max-weight node
		}
	}

	// get all max-weight nodes for random select
	std::vector<std::string> max_w_nodes_vec;
	for (it = node_map.begin(); it != node_map.end(); ++it) {
		int w = it->second.weight;
		if (w == max_w) {
			if (it->first == stats_info.disable_node_) {
				LOG("disable_node[%s]", stats_info.disable_node_);
				continue;
			}
			max_w_nodes_vec.push_back(it->first);
			LOG("push_back max node[%s]", it->first.c_str());
		}
	}

	if (max_w_nodes_vec.empty()) {
		err_msg = "init, no valid nodes";
		return TDF_STATS_S_FAILED;
	}

	int random_one = rand() % max_w_nodes_vec.size();
	g_node_selected = max_w_nodes_vec[random_one];

	LOG("get random_one[%d] g_node_selected[%s] max_w_nodes_vec.size[%u]", 
			random_one, g_node_selected.c_str(), max_w_nodes_vec.size());

	return TDF_STATS_OK;
}

static int update_wrr(stats_info_t &stats_info)
{
	std::string service =  stats_info.service_;

	g_ready_nodes.clear();
	g_detect_nodes.clear();

	std::map<std::string, node_attr_t> &node_map =  g_stats_shm_conf_map[service];
	service_attr_t &service_attr                 =  g_stats_shm_conf_service_attr_map[service];

	std::map<std::string, node_attr_t>::iterator it = node_map.begin();
	for (; it != node_map.end(); ++it) {
		int w = it->second.weight;
		int s = it->second.status;

		if (s == E_NODE_STATUS_OK) {
			node_t node(it->first, w);
			g_ready_nodes.push_back(node);

		} else if (s == E_NODE_STATUS_PROBE) {
			node_t node(it->first, w);
			g_detect_nodes.push_back(node);
		}
	}

	// sort by weight, such as ip1:3 ip2:2 ip3:1
	std::sort(g_ready_nodes.begin(), g_ready_nodes.end(), node_compare);

	// select a random detect node 
	//std::sort(g_detect_nodes.begin(), g_detect_nodes.end(), node_compare);// weight is all the same, now is 1

	assert(!g_ready_nodes.empty());
	g_max_w =  g_ready_nodes[0].weight;
	g_gcd   =  g_ready_nodes.back().weight;

	// calc gcd for all nodes
	std::vector<node_t>::iterator beg = g_ready_nodes.begin();
	std::vector<node_t>::iterator end = g_ready_nodes.end();

	int w1, w2;
	for (; beg != end && g_gcd > 1; ++beg) {
		w1 = beg->weight;
		w2 = g_gcd;
		GET_GCD(w1, w2, g_gcd);
	} 

	return TDF_STATS_OK;
}

static int proc_wrr(stats_info_t &stats_info, std::string &err_msg)
{
	int ret = TDF_STATS_OK;

	std::string disable_node =  stats_info.disable_node_;

	if (g_need_update) {
		update_wrr(stats_info);
	}

	// select a random detect node
	if (disable_node.empty()
			&& !g_detect_nodes.empty()
			&& is_maybe_true(g_perhaps)) {

		int index   =  rand() % g_detect_nodes.size();
		node_t node =  g_detect_nodes[index];
		g_detect_nodes.erase(g_detect_nodes.begin() + index);

		g_node_selected = node.host;
		LOG("get a random node from detect nodes, index[%d] g_node_selected[%s]", index, g_node_selected.c_str());                
		return TDF_STATS_OK;
	}

	int size = g_ready_nodes.size();
	if (size == 1) {
		g_node_selected = g_ready_nodes[0].host;
		LOG("ready nodes only has one, g_node_selected[%s]", g_node_selected.c_str());                
		return TDF_STATS_OK;
	}

	g_last_index = g_last_index % size;

	for (;;) {
		if (g_ready_nodes[g_last_index].weight > g_current_weight) {

			g_node_selected = g_ready_nodes[g_last_index].host;
			LOG("foreach ready nodes, g_last_index[%d] g_node_selected[%s]", 
					g_last_index, g_node_selected.c_str());                

			// next find 
			g_last_index = (g_last_index + 1) % size;
			if (0 == g_last_index) {
				g_current_weight = (g_current_weight >= g_max_w) ? 0 : (g_current_weight + g_gcd);
			}

			// this maybe happen
			if (g_node_selected == disable_node) {
				LOG("foreach ready nodes, g_last_index[%d] g_node_selected[%s] == disable_node[%s], so need to find next", 
						g_last_index - 1, g_node_selected.c_str(), disable_node.c_str());                
				continue;
			}

			return TDF_STATS_OK;
		}
		g_current_weight = (g_current_weight >= g_max_w) ? 0 : (g_current_weight + g_gcd);
		g_last_index = 0;
	}

	return TDF_STATS_OK;
}

static int update_sp(stats_info_t &stats_info)
{
	std::string service =  stats_info.service_;

	g_ready_nodes.clear();
	g_detect_nodes.clear();
	g_sp_retry_nodes.clear();

	g_ready_groups.clear();
	g_detect_groups.clear();

	std::map<std::string, node_attr_t> &node_map =  g_stats_shm_conf_map[service];
	service_attr_t &service_attr                 =  g_stats_shm_conf_service_attr_map[service];

	std::map<std::string, node_attr_t>::iterator it = node_map.begin();
	for (; it != node_map.end(); ++it) {
		int w = it->second.weight;
		int s = it->second.status;

		if (s == E_NODE_STATUS_OK) {
			//node_t node(it->first, w);
			//g_ready_nodes.push_back(node);

			std::vector<std::string> &nodes = g_ready_groups[w];
			nodes.push_back(it->first);

		} else if (s == E_NODE_STATUS_PROBE) {
			//node_t node(it->first, w);
			//g_detect_nodes.push_back(node);

			std::vector<std::string> &nodes = g_detect_groups[w];
			nodes.push_back(it->first);
		}
	}

	// set g_ready_nodes 
	int get_sp_retry_nodes = 0;
	std::map<int, std::vector<std::string> >::reverse_iterator r_rit = g_ready_groups.rbegin();
	for (; r_rit != g_ready_groups.rend(); ++r_rit) {

		std::vector<std::string> &nodes = r_rit->second;
		std::vector<std::string>::iterator node_it = nodes.begin();
		for (; node_it != nodes.end(); ++node_it) {
			node_t node(*node_it, r_rit->first);
			if (get_sp_retry_nodes) {
				g_sp_retry_nodes.push_back(node);
			} else {
				g_ready_nodes.push_back(node);
			}
		}

		if (get_sp_retry_nodes == 0
				&& g_ready_nodes.size() == 1) {
			LOG("get lower weight nodes to prevent retring same node when there is only one node in g_ready_nodes[%s,%d]", 
					g_ready_nodes[0].host.c_str(), g_ready_nodes[0].weight);
			get_sp_retry_nodes = 1;
			continue;

		} else {
			break;
		}
	}
	LOG("set g_ready_nodes size[%u] g_sp_retry_nodes size[%u]", 
			g_ready_nodes.size(), g_sp_retry_nodes.size());

	// set g_detect_nodes, it's weight is 1
	std::map<int, std::vector<std::string> >::reverse_iterator r_dit = g_detect_groups.rbegin();
	for (; r_dit != g_detect_groups.rend(); ++r_dit) {

		std::vector<std::string> &nodes = r_dit->second;
		std::vector<std::string>::iterator node_it = nodes.begin();
		for (; node_it != nodes.end(); ++node_it) {
			node_t node(*node_it, r_dit->first);
			g_detect_nodes.push_back(node);
		}

		// get last one, that is max weight node 
		break;
	}

	// sort by weight, such as ip1:3 ip2:2 ip3:1
	std::sort(g_ready_nodes.begin(), g_ready_nodes.end(), node_compare);

	// select a random detect node 
	//std::sort(g_detect_nodes.begin(), g_detect_nodes.end(), node_compare);// weight is all the same, now is 1

	assert(!g_ready_nodes.empty());
	g_max_w =  g_ready_nodes[0].weight;
	g_gcd   =  g_ready_nodes.back().weight;

	// calc gcd for all nodes
	std::vector<node_t>::iterator beg = g_ready_nodes.begin();
	std::vector<node_t>::iterator end = g_ready_nodes.end();

	int w1, w2;
	for (; beg != end && g_gcd > 1; ++beg) {
		w1 = beg->weight;
		w2 = g_gcd;
		GET_GCD(w1, w2, g_gcd);
	} 

	return TDF_STATS_OK;
}

static int proc_sp(stats_info_t &stats_info, std::string &err_msg)
{
	int ret = TDF_STATS_OK;

	std::string disable_node =  stats_info.disable_node_;

	if (g_need_update) {
		update_sp(stats_info);
	}

	// select a random detect node 
	if (disable_node.empty()
			&& !g_detect_nodes.empty()
			&& is_maybe_true(g_perhaps)) {

		int index   =  rand() % g_detect_nodes.size();
		node_t node =  g_detect_nodes[index];
		g_detect_nodes.erase(g_detect_nodes.begin() + index);

		g_node_selected = node.host;
		LOG("get a random node from detect nodes, index[%d] g_node_selected[%s]", index, g_node_selected.c_str());                
		return TDF_STATS_OK;
	}

	int size = g_ready_nodes.size();
	if (size == 1) {
		g_node_selected = g_ready_nodes[0].host;
		if (g_node_selected == disable_node
				&& !g_sp_retry_nodes.empty()) {

			int index       =  rand() % g_sp_retry_nodes.size();
			node_t node     =  g_sp_retry_nodes[index];
			g_node_selected =  node.host;
			LOG("ready nodes only has one and is unconnected so get a random node from g_sp_retry_nodes, g_node_selected[%s]", g_node_selected.c_str());  

		} else {
			LOG("ready nodes only has one, g_node_selected[%s]", g_node_selected.c_str());                
		}

		return TDF_STATS_OK;
	}

	g_last_index = g_last_index % size;

	for (;;) {
		if (g_ready_nodes[g_last_index].weight > g_current_weight) {

			g_node_selected = g_ready_nodes[g_last_index].host;
			LOG("foreach ready nodes, g_last_index[%d] g_node_selected[%s]", 
					g_last_index, g_node_selected.c_str());                

			// next find 
			g_last_index = (g_last_index + 1) % size;
			if (0 == g_last_index) {
				g_current_weight = (g_current_weight >= g_max_w) ? 0 : (g_current_weight + g_gcd);
			}

			// this maybe happen
			if (g_node_selected == disable_node) {
				LOG("foreach ready nodes, g_last_index[%d] g_node_selected[%s] == disable_node[%s], so need to find next", 
						g_last_index - 1, g_node_selected.c_str(), disable_node.c_str());                
				continue;
			}

			return TDF_STATS_OK;
		}
		g_current_weight = (g_current_weight >= g_max_w) ? 0 : (g_current_weight + g_gcd);
		g_last_index = 0;
	}

	return TDF_STATS_OK;
}

static int load_shm(stats_info_t &stats_info, std::string &stats_shm_conf, std::string &err_msg)
{
	int ret = TDF_STATS_OK;

	try {
		// usually path is /titan/stats
		scoped_ptr<NCS::shm_config> conf(new NCS::shm_config(stats_shm_conf, true));

		std::string mode =  (*conf)[ROOT_NODE][stats_info.service_]("m");
		std::string ver  =  (*conf)[ROOT_NODE][stats_info.service_]("ver");

		g_need_update = 0;
		if (g_mode.empty() && g_ver.empty()) {
			LOG("g_mode.empty && g_ver.empty, need update");
			g_need_update = 1;

		} else if ((!g_mode.empty() && g_mode != mode)
				|| (!g_ver.empty() && g_ver != ver)) {
			LOG("g_mode[%s] != mode[%s] or g_ver[%s] != ver[%s], need update", 
					g_mode.c_str(), mode.c_str(), g_ver.c_str(), ver.c_str());
			g_need_update = 1;

		} else {
			LOG("no need update, g_mode[%s] g_ver[%s]",
					g_mode.c_str(), g_ver.c_str());
		}
		g_mode =  mode;
		g_ver  =  ver;

		if (g_need_update) {
			std::map<std::string, node_attr_t> node_map;
			NCS::shm_config::tree_type node_list    =  (*conf)[ROOT_NODE][stats_info.service_].get_tree();
			NCS::shm_config::tree_type::iterator it =  node_list.begin();

			for (; it != node_list.end(); ++it) {
				std::string node = *it;
				int w =  atoi((*conf)[ROOT_NODE][stats_info.service_][node]("w").c_str());
				int s =  atoi((*conf)[ROOT_NODE][stats_info.service_][node]("s").c_str());

				node_attr_t node_attr(w, s);       
				node_map[node] = node_attr;
			}

			g_stats_shm_conf_map[stats_info.service_] = node_map;

			service_attr_t service_attr(g_mode, g_ver);
			g_stats_shm_conf_service_attr_map[stats_info.service_] = service_attr;
		}

	} catch (NCS::ncs_exception e) {
		err_msg = e.what();
		return TDF_STATS_S_FAILED;
	}

	return TDF_STATS_OK;
}

static void init_log(stats_info_t &stats_info, std::string &enable_stats_client_log)
{
	g_uuid = stats_info.self_;

	if (enable_stats_client_log == "enable") {
		g_enable_stats_client_log = 1;

	} else {
		g_enable_stats_client_log = 0;
	}
	return;
}

static void init_req()
{
	static int once = 1;
	if (once) {
		once = 0;

		srand(time(NULL));// initialize the random seed
		g_last_index     =  0;
		g_current_weight =  0;
	}

	g_node_selected.clear();
	return;
}

int get_stats(stats_info_t &stats_info, std::string &stats_shm_conf, std::string &enable_stats_client_log, std::string &err_msg)
{
	int ret = TDF_STATS_OK;

	init_log(stats_info, enable_stats_client_log);

	LOG("<-------------- req: service[%s] func[%s] disable_node[%s]", 
			stats_info.service_, stats_info.function_, stats_info.disable_node_);

	init_req();

	ret = load_shm(stats_info, stats_shm_conf, err_msg);
	if (ret != TDF_STATS_OK) {
		return ret;
	}

	if (g_stats_shm_conf_map.count(stats_info.service_) == 0) {
		err_msg = std::string(stats_info.service_) + "not found";
		return TDF_STATS_S_FAILED;
	}

	if (g_mode == "INIT") {
		ret = proc_init(stats_info, err_msg);
		if (ret != TDF_STATS_OK) {
			return ret;
		}

	} else if (g_mode == "WRR") {
		ret = proc_wrr(stats_info, err_msg);
		if (ret != TDF_STATS_OK) {
			return ret;
		}

	} else if (g_mode == "SP") {
		ret = proc_sp(stats_info, err_msg);
		if (ret != TDF_STATS_OK) {
			return ret;
		}

	} else {
		err_msg = g_mode + " is invalid";
		return TDF_STATS_S_FAILED;
	}

	if (g_node_selected.empty()) {
		err_msg = "get ip err";
		return TDF_STATS_S_FAILED;
	}

	std::string::size_type pos =  g_node_selected.find(":");
	std::string ip             =  g_node_selected.substr(0, pos);
	std::string port           =  g_node_selected.substr(pos + 1);

	snprintf(stats_info.host_ip_,     sizeof(stats_info.host_ip_),     "%s",   ip.c_str());
	snprintf(stats_info.host_port_,   sizeof(stats_info.host_port_),   "%s",   port.c_str());

	return TDF_STATS_OK;
}

int report_stats(stats_info_t& stats_info, int ret, int usetime_usec, std::string& err_msg)
{
	LOG("--------------> req: service[%s] func[%s] disable_node[%s]", 
			stats_info.service_, stats_info.function_, stats_info.disable_node_);

	fifo worker2stats_fifo;
	try {
		worker2stats_fifo.open("work2stats.fifo", fifo::EM_WRITE);

	}  catch (fifo_fail& e) {
		err_msg = e.what();
		return TDF_STATS_S_FIFO_OPEN_WR_FAILED;
	}

	callee_info_t callee_info;
	snprintf(callee_info.caller_self_,        sizeof(callee_info.caller_self_),        "%s",   stats_info.self_);
	snprintf(callee_info.callee_service_,     sizeof(callee_info.callee_service_),     "%s",   stats_info.service_);
	snprintf(callee_info.callee_function_,    sizeof(callee_info.callee_function_),    "%s",   stats_info.function_);
	snprintf(callee_info.callee_host_ip_,     sizeof(callee_info.callee_host_ip_),     "%s",   stats_info.host_ip_);
	snprintf(callee_info.callee_host_port_,   sizeof(callee_info.callee_host_port_),   "%s",   stats_info.host_port_);

	callee_info.callee_ret_          =  ret;
	callee_info.callee_usetime_usec_ =  usetime_usec;

	try {
		worker2stats_fifo.write((char *)&callee_info, sizeof(callee_info));

	} catch (fifo_delay& e) {
		err_msg = e.what();
		return TDF_STATS_S_FIFO_WRITE_FAILED;

	} catch (fifo_fail& e) {
		err_msg = e.what();
		return TDF_STATS_S_FIFO_WRITE_FAILED;
	}

	return 0;
}

