#include "db_handle.h"

using namespace MYDB;

CMyDB::CMyDB()
{
}

CMyDB::~CMyDB()
{
}

int CMyDB::init(const std::string &config, const std::string &strDBType)
{
	int iRet       =  E_OK;
	iRet           =  m_stDBConn.init(config, strDBType);
	m_iErrorNo     =  m_stDBConn.getErrorNo();
	m_strErrorInfo =  m_stDBConn.getErrorInfo();

	return iRet;
}

int CMyDB::getErrorNo()
{
	return m_iErrorNo;
}

string CMyDB::getErrorInfo()
{
	return m_strErrorInfo;
}

void CMyDB::ping()
{
	m_stDBConn.ping();
}

int CMyDB::setOrder(Record &record)
{
	int iRet = E_OK;

	return E_OK;
}


