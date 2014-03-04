#include "CAppConfig.h"

CAppConfig* CAppConfig::instance = NULL;

CAppConfig& CAppConfig::getapp_config_instance()
{
	if (instance == NULL) {
		instance = new CAppConfig();
	}

	return *instance;
}

void CAppConfig::check_conf() throw (runtime_error)
{
	if (m_strProjectHome == "") throw runtime_error("please check option projecthome");
	if (m_ithreadcnt == 0) throw runtime_error("please check option threadcnt");
	if (m_iprocesscnt == 0) throw runtime_error("please check option processcnt");
	if (m_irequestcnt == 0) throw runtime_error("please check option requestcnt");

	cout << "------------------- Paras info [beg] -------------------" << endl;
	cout << "projecthome[" << m_strProjectHome.c_str() << "]" << endl;
	cout << "threadcnt[" << m_ithreadcnt << "] processcnt[" << m_iprocesscnt << "] requestcnt[" << m_irequestcnt << "]" << endl;
	cout << "------------------- Paras info [end] -------------------" << endl;

	return;
}
