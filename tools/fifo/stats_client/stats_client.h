#ifndef TDF_STATS_CLIENT_H_
#define TDF_STATS_CLIENT_H_

#include <string>

enum tdf_runtime_errno_e
{
	TDF_STATS_OK =  0,

	// system err
	TDF_STATS_S_FAILED              =  -1000,
	TDF_STATS_S_FIFO_OPEN_WR_FAILED =  -1001,
	TDF_STATS_S_FIFO_WRITE_FAILED   =  -1002,

	// non-system err
	TDF_STATS_NS_FAILED         =  -2000,
	TDF_STATS_NS_NO_NEED_UPDATE =  -2001,
};

typedef struct stats_info_s
{
	char self_[48];              // self info
	char service_[64];           // remote service
	char function_[64];          // remote function
	char host_ip_[16];           // remote ip
	char host_port_[8];          // remote port
	char disable_node_[24];      // disable

	stats_info_s(){}

} stats_info_t;

typedef struct callee_info_s
{
	char caller_self_[48];            // self info
	char callee_service_[64];         // remote service
	char callee_function_[64];        // remote function
	char callee_host_ip_[16];         // remote ip
	char callee_host_port_[8];        // remote port
	int callee_ret_;                   
	int callee_usetime_usec_;                   

	callee_info_s() : callee_ret_(0), callee_usetime_usec_(0) {}

} callee_info_t;

/* 
 * Description
 * 
 * Return Value
 * On success, 0 is returned.
 * On error, <0 is returned, and err_msg is set appropriately. 
 */
int get_stats(stats_info_t &stats_info, std::string &stats_shm_conf, std::string &enable_stats_client_log, std::string &err_msg);

/* 
 * Description
 * ret: 0 for nomal, <0 for abnormal
 * usetime_usec: time used
 * 
 * Return Value
 * On success, 0 is returned.
 * On error, <0 is returned, and err_msg is set appropriately. 
 */
int report_stats(stats_info_t &stats_info, int ret, int usetime_usec, std::string &err_msg);



#endif /* TDF_STATS_CLIENT_H_ */

