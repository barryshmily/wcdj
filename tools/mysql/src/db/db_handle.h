#ifndef _DB_HANDLE_H_
#define _DB_HANDLE_H_

#include "dbbase.h"

namespace MYDB 
{

	#define DB_NAME             "test_conf"
	#define TABLE_NAME          "t_my_order"

	#define ORDER_ORDERID   "FOrderId"
	#define ORDER_QQUIN     "FQQUin"


	class CMyDB
	{
	public:
		CMyDB();
		~CMyDB();

		int getErrorNo(void);
		string getErrorInfo(void);

		void ping(void);

		int init(const std::string &config, const std::string &strDBType);

		int setOrder(Record &record);

		int getOrder(Record &record, Record &order);

		int updateOrder(Record &record);


	private:
		DBConn m_stDBConn;
		int m_iErrorNo;
		string m_strErrorInfo;

		string m_strDBName;
	};
}

#endif /* _DB_HANDLE_H_ */
