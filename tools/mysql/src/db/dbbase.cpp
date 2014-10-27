#include "dbbase.h"
using namespace MYDB;

ConnItem::ConnItem()
{
	m_iIndex  =  -1;
	m_iPort   =  0;
	m_iStatus =  -2;  // 初始状态

	m_pLink = NULL;
}

ConfInfo::ConfInfo()
{
	m_iIndex  =  0;
	m_iPort   =  0;
	m_iStatus =  0;
}

DBConn::DBConn()
{
	m_iErrorNo     =  0;
	m_strErrorInfo =  "OK";
}

DBConn::~DBConn()
{
	destroyConn();
}

int DBConn::init(const std::string &config, const std::string &strDBType)
{
	if (config.empty())
	{
		m_iErrorNo     =  DB_CONF_NOT_EXIST;
		m_strErrorInfo =  "DBConf is Null!";
		return -1;
	}

	m_strConfigFile = config;
	if (loadConf(strDBType) < 0)
	{
		return -1;
	}

	if (initConn() < 0)
	{
		return -1;
	}

	return 0;
}

int DBConn::loadConf(const std::string &strDBType)
{
	// TODO 可以配置多个连接

	ConfInfo confInfo;
	confInfo.m_strHost   =  "127.0.0.1";
	confInfo.m_strPasswd =  "gerry";
	confInfo.m_strUser   =  "gerry";
	confInfo.m_iPort     =  3306;

	confInfo.m_iStart =  0;
	confInfo.m_iEnd   =  0;

	confInfo.m_iStatus =  0;
	confInfo.m_iIndex  =  0;

	m_vConfInfo.push_back(confInfo);

	m_iErrorNo     =  0;
	m_strErrorInfo =  "OK";

	return 0;
}

int DBConn::initConn()
{
	// 根据每个配置节点创建数据库连接
	std::vector<ConfInfo>::iterator iter = m_vConfInfo.begin();
	for(; iter != m_vConfInfo.end(); ++iter)
	{
		ConnItem connItem;
		connItem.m_strHost   =  iter->m_strHost;
		connItem.m_strUser   =  iter->m_strUser;
		connItem.m_strPasswd =  iter->m_strPasswd;
		connItem.m_iPort     =  iter->m_iPort;

		connItem.m_pLink = mysql_init(NULL);
		if(!connItem.m_pLink) // 失败
		{
			m_iErrorNo     =  DB_INIT_ERR;
			m_strErrorInfo =  "init DB ERROR";
			destroyConn();
			return -1;
		}

		connItem.m_iStatus = -1;

		int timeout = 3;
		mysql_options(connItem.m_pLink,   MYSQL_OPT_CONNECT_TIMEOUT,   (const char *)&timeout);
		mysql_options(connItem.m_pLink,   MYSQL_OPT_READ_TIMEOUT,      (const char *)&timeout);
		mysql_options(connItem.m_pLink,   MYSQL_OPT_WRITE_TIMEOUT,     (const char *)&timeout);
		if(connect(connItem) < 0)
		{
			destroyConn();
			return -1;
		}

		connItem.m_iIndex  =  iter->m_iIndex;
		connItem.m_iStatus =  0;

		m_vConn.push_back(connItem);
	}

	return 0;
}

void DBConn::destroyConn()
{
	std::vector<ConnItem>::iterator iter = m_vConn.begin();
	for(; iter != m_vConn.end(); ++iter)
	{
		if(iter->m_pLink)
		{
			mysql_close(iter->m_pLink);
			iter->m_pLink = NULL;
		}
	}
}

int DBConn::connect(ConnItem &item)
{
	if (item.m_pLink)
	{
		mysql_close(item.m_pLink);
		item.m_pLink = NULL;
	}

	item.m_pLink = mysql_init(NULL);
	if (!item.m_pLink)
	{
		m_iErrorNo     =  DB_INIT_ERR;
		m_strErrorInfo =  "init DB ERROR";
		return -1;
	}

	int timeout = 3;
	mysql_options(item.m_pLink,   MYSQL_OPT_CONNECT_TIMEOUT,   (const char *)&timeout);
	mysql_options(item.m_pLink,   MYSQL_OPT_READ_TIMEOUT,      (const char *)&timeout);
	mysql_options(item.m_pLink,   MYSQL_OPT_WRITE_TIMEOUT,     (const char *)&timeout);

	if (!mysql_real_connect(item.m_pLink, item.m_strHost.c_str(), item.m_strUser.c_str(), item.m_strPasswd.c_str(), 
				NULL, item.m_iPort, NULL, 0))
	{
		m_iErrorNo     =  DB_CONNECTION_ERR;
		m_strErrorInfo =  mysql_error(item.m_pLink);

		mysql_close(item.m_pLink);
		item.m_pLink = NULL;
		return -1;
	}

	return 0;

}

int DBConn::reconnect(int index, int error_no)
{
	if (index < 0)
	{
		m_iErrorNo     =  DB_INDEX_ERR;
		m_strErrorInfo =  "db index error";
		return -1;
	}


	if (error_no == CR_SERVER_GONE_ERROR || error_no ==CR_SERVER_LOST)
	{
		return connect(m_vConn[index]);
	}

	return 0;
}

MYSQL *DBConn::getConn(unsigned dbBit, int &index)
{
	m_iErrorNo     =  0;
	m_strErrorInfo =  "OK";

	if (m_vConfInfo.empty() == true)
	{
		goto error_exit;
	}

	if (TC_DB_DIVIDE_NO == dbBit)
	{
		index = 0;

		//连接为空则重新连接
		if (!m_vConn[index].m_pLink)
		{
			//重连
			if (connect(m_vConn[index]))
				return NULL;
		}

		return m_vConn[index].m_pLink;
	}
	else
	{
		// find start <= temp <= end
		std::vector<ConfInfo>::iterator iter = m_vConfInfo.begin();
		for (; iter != m_vConfInfo.end(); ++iter)
		{
			if (iter->m_iStart <= (int)dbBit && iter->m_iEnd >= (int)dbBit)
			{
				index = iter->m_iIndex;

				//连接为空则重新连接
				if (!m_vConn[index].m_pLink)
				{
					//重连
					if (connect(m_vConn[index]))
						return NULL;
				}

				return m_vConn[index].m_pLink;
			}
		}
	}

error_exit:
	// no found
	char msg[100] = {0};
	snprintf(msg, sizeof(msg), "the DB can't find, please check the db conf");

	m_iErrorNo     =  DB_NOT_EXIST;
	m_strErrorInfo =  msg;

	return NULL;
}

void DBConn::releaseConn(int index)
{
	if (index >= m_vConn.size() || index < 0)
	{
		m_iErrorNo     =  DB_INDEX_ERR;
		m_strErrorInfo =  "db index error";
		return;
	}

	m_vConn[index].m_iStatus = 0;
}

int DBConn::getErrorNo()
{
	return m_iErrorNo;
}

const char *DBConn::getErrorInfo()
{
	return m_strErrorInfo.c_str();
}

int DBConn::execQuery(const char *szSql, MYSQL *mysql, MYSQL_RES *&result_set, int iDBIndex)
{
	m_iErrorNo     =  0;
	m_strErrorInfo =  "OK";

	if (0 != mysql_query(mysql, szSql))
	{
		int iError = mysql_errno(mysql);

		char szError[1024]= {0};
		snprintf(szError, sizeof(szError) - 1, "mysql_query ERROR[%d][%s]", mysql_errno(mysql), mysql_error(mysql));

		reconnect(iDBIndex, iError);

		m_strErrorInfo =  szError;
		m_iErrorNo     =  iError;
		return -1;
	}

	if (NULL == (result_set = mysql_store_result(mysql)))
	{
		int iError = mysql_errno(mysql);

		char szError[1024] = {0};
		snprintf(szError, sizeof(szError) - 1, "mysql_store_result ERROR[%d][%s]", mysql_errno(mysql), mysql_error(mysql));

		reconnect(iDBIndex, iError);

		m_strErrorInfo =  szError;
		m_iErrorNo     =  iError;
		return -1;
	}

	return 0;
}

int DBConn::execInsertUpdate(const char *szSql, MYSQL *mysql, int iDBIndex)
{
	m_iErrorNo     =  0;
	m_strErrorInfo =  "OK";

	if (0 != mysql_query(mysql, szSql))
	{
		int iError = mysql_errno(mysql);

		char szError[1024] = {0};
		snprintf(szError, sizeof(szError), "mysql_query ERROR[%d][%s]", mysql_errno(mysql), mysql_error(mysql));

		reconnect(iDBIndex, iError);

		m_strErrorInfo =  szError;
		m_iErrorNo     =  iError;
		return -1;
	}

	return 0;
}

int DBConn::execInsertUpdate(const char *szSql, MYSQL *mysql, int iDBIndex, int &iAffectRow)
{
	m_iErrorNo     =  0;
	m_strErrorInfo =  "OK";
	iAffectRow     =  0;

	if (0 != mysql_query(mysql, szSql))
	{
		int iError = mysql_errno(mysql);
		iAffectRow = mysql_affected_rows(mysql);

		char szError[1024] = {0};
		snprintf(szError, sizeof(szError) - 1, "mysql_query ERROR[%d][%s]", mysql_errno(mysql), mysql_error(mysql));

		reconnect(iDBIndex, iError);

		m_strErrorInfo =  szError;
		m_iErrorNo     =  iError;
		return -1;
	}

	iAffectRow = mysql_affected_rows(mysql);

	return 0;
}

void DBConn::ping(void)
{
	std::vector<ConnItem>::iterator iter = m_vConn.begin();
	for(; iter != m_vConn.end(); ++iter)
	{
		if(iter->m_pLink == NULL)
		{
			continue;
		}
		mysql_ping(iter->m_pLink);
	}
}

