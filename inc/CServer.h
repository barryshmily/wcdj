#ifndef __WCDJ_SERVER_H__
#define __WCDJ_SERVER_H__

#include <unistd.h>
#include <stdexcept>
#include <ctime>
#include "CSV_message_queue.h"
#include "CAppConfig.h"

using namespace std;

extern const unsigned char SIG_SHOW;
extern const unsigned char SIG_QUIT;
extern unsigned char SIG_STAT;


class CServer 
{
public:
	CServer();
	~CServer();

	void init(CAppConfig &appconf_instance) throw(runtime_error, logic_error);
	void run();

	// SV-MQ operations
	void init_svmq(CAppConfig &appconf_instance) throw (runtime_error);
	void create_svmq(CSVMessageQueue * &pCSVMessageQueue, int iSvMqKey) throw (runtime_error);
	int enqueue(CSVMessageQueue * &pCSVMessageQueue, const void* vData, int iDataLen, int flag = IPC_NOWAIT);
	int dequeue(CSVMessageQueue * &pCSVMessageQueue, void *vData, int iDataLen, int flag = IPC_NOWAIT);

	void clean_up();


private:
	CSVMessageQueue *m_pCSVMessageQueueClient;
	CSVMessageQueue *m_pCSVMessageQueueServer;
	char m_szErrInfo[512];

	int m_iClientSvMqKey;
	int m_iServerSvMqKey;
};

#endif
