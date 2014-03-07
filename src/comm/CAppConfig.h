#ifndef __WCDJ_CONFIG_H_
#define __WCDJ_CONFIG_H_

#include <cstdio>
#include <iostream>
#include <stdexcept>
using namespace std;

class CAppConfig
{
public:
	~CAppConfig() {}

	// singleton pattern
	static CAppConfig& getapp_config_instance();

	void check_conf() throw (runtime_error);

	void set_projecthome(const char *v) { m_strprojecthome = v; }
	void set_clientsvmqkey(int v) { m_iclient_svmqkey = v; }
	void set_serversvmqkey(int v) { m_iserver_svmqkey = v; }


	string& get_projecthome() { return m_strprojecthome; }
	int& get_clientsvmqkey() { return m_iclient_svmqkey; }
	int& get_serversvmqkey() { return m_iserver_svmqkey; }


private:
	// private ctor
	CAppConfig() {}
	CAppConfig(const CAppConfig &s) {}
	void operator=(const CAppConfig &rhs) {}

private:

	static CAppConfig* instance;

	string m_strprojecthome;

	// SV-MQ key
	// Note, we can create or start many MiddleSvr to transmit msg by SV-MQ
	int m_iclient_svmqkey;
	int m_iserver_svmqkey;

};

#endif
