#ifndef _DBBASE_H_
#define _DBBASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"
#include <errmsg.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

#define MAX_LEN_SQL     2048*5
#define TC_DB_DIVIDE_NO 0xffffffff // 数据库不分库

#define Record map<string, string>
#define E_OK 0
#define E_ERR -1

//1000~2999 mysql errno

#define DB_CONF_NOT_EXIST				3001
#define DB_INIT_ERR						3002
#define DB_INDEX_ERR					3003
#define DB_NOT_EXIST					3004
#define DB_CONNECTION_ERR				3005
#define DB_FILE_PARSE_ERR				3006
#define DB_FETCH_ROW_ERR				3007



namespace MYDB
{
	/**
	 * 数据库连接节点
	 */
	struct ConnItem
	{
		int m_iStatus; // -2 初始状态, -1 完成init 操作, 0 完成connect 操作 1 被占用
		int m_iIndex;

		int m_iPort;
		std::string m_strHost;
		std::string m_strUser;
		std::string m_strPasswd;
		std::string m_strDBName;
		MYSQL *m_pLink;
		ConnItem();
	};

	/**
	 * 数据库配置信息
	 */
	struct ConfInfo
	{
		int m_iType;
		int m_iStatus;
		int m_iIndex;

		int m_iPort;

		int m_iStart;
		int m_iEnd;

		std::string m_strHost;
		std::string m_strUser;
		std::string m_strPasswd;
		ConfInfo();
	};

	/**
	 * 数据库连接类
	 */
	class DBConn
	{
	private:
		std::vector<ConfInfo> m_vConfInfo;
		std::vector<ConnItem> m_vConn;

		std::string m_strConfigFile;

		int m_iErrorNo;
		std::string m_strErrorInfo;

	public:
		DBConn();
		~DBConn();

		/**
		 * 读取数据库配置文件，并连接数据库
		 * @param config [in] 数据库配置文件
		 * @param strDBType [in] 配置文件中的tag标签
		 * @return 0成功 其他失败
		 */
		int init(const std::string &config, const std::string &strDBType);

		/**
		 * 获取数据库连接
		 * @param dbBit [in] 数据库分库索引号
		 * @param index [out] 数据库连接索引号
		 * @return 0成功 其他失败
		 */
		MYSQL* getConn(unsigned dbBit, int &index);
		/**
		 * 释放数据库连接
		 * @param index [in] 数据库连接索引号
		 * @return 0成功 其他失败
		 */
		void releaseConn(int index);
		/**
		 * 重连数据库库
		 * @param index [in] 数据库连接索引号
		 * @return 0成功 其他失败
		 */
		int reconnect(int index, int error_no);

		/**
		 * 执行查询操作
		 * @param szSql [in] 执行的SQL语句
		 * @param mysql [out] mysql句柄
		 * @param result_set [out] 执行的mysql结果集
		 * @param iDBIndex [in] 数据库索引号，数据库不分库则为PUBLISH_DB_DIVIDE_NO
		 * @return 0成功 其他失败
		 */
		int execQuery(const char *szSql, MYSQL *mysql, MYSQL_RES *&result_set,
				int iDBIndex);

		/**
		 * 执行插入修改删除数据库操作
		 * @param szSql [in] 执行的SQL语句
		 * @param mysql [out] mysql句柄
		 * @param iDBIndex [in] 数据库索引号，数据库不分库则为PUBLISH_DB_DIVIDE_NO
		 * @return 0成功 其他失败
		 */
		int execInsertUpdate(const char *szSql, MYSQL *mysql, int iDBIndex);

		int execInsertUpdate(const char *szSql, MYSQL *mysql, int iDBIndex, int &iAffectRow);

		void ping(void);

		int getErrorNo();
		const char *getErrorInfo();

	private:

		DBConn(const DBConn &);
		DBConn &operator=(const DBConn &);

	private:
		int loadConf(const std::string &strDBType);
		/**
		 * 根据每个配置节点创建数据库连接
		 * @return 0成功 其他失败
		 */
		int initConn(void);

		/**
		 * 配置节点连接
		 * @param item [in] 配置节点
		 * @return 0成功 其他失败
		 */
		int connect(ConnItem &item);

		/**
		 * 为每个配置节点断开数据库连接
		 * @return 0成功 其他失败
		 */
		void destroyConn();

	};
}

#endif /* _DBBASE_H_ */
