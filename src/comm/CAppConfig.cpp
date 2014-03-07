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
	if (m_strprojecthome == "") throw runtime_error("please check option projecthome");
	if (m_iclient_svmqkey == 0) throw runtime_error("please check option clientsvmqkey");
	if (m_iserver_svmqkey == 0) throw runtime_error("please check option serversvmqkey");


	cout << "------------------- Paras info [beg] -------------------" << endl;
	cout << "projecthome[" << m_strprojecthome.c_str() << "]" << endl;
	cout << "clientsvmqkey[" << m_iclient_svmqkey << "] serversvmqkey[" << m_iserver_svmqkey << "]" << endl;
	cout << "------------------- Paras info [end] -------------------" << endl;

	return;
}
