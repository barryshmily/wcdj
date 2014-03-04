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

	void set_projecthome(const char *v) { m_strProjectHome = v; }
	void set_threadcnt(unsigned v) { m_ithreadcnt = v; }
	void set_processcnt(unsigned v) { m_iprocesscnt = v; }
	void set_requestcnt(unsigned v) { m_irequestcnt = v; }

	string& get_projecthome() { return m_strProjectHome; }
	unsigned& get_threadcnt() { return m_ithreadcnt; }
	unsigned& get_processcnt() { return m_iprocesscnt; }
	unsigned& get_requestCnt() { return m_irequestcnt; }


private:
	// private ctor
	CAppConfig() {}
	CAppConfig(const CAppConfig &s) {}
	void operator=(const CAppConfig &rhs) {}

private:

	static CAppConfig* instance;

	string m_strProjectHome;
	unsigned  m_ithreadcnt;
	unsigned  m_iprocesscnt;
	unsigned  m_irequestcnt;


};

#endif
