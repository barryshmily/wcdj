#ifndef __WCDJ_SERVER_H__
#define __WCDJ_SERVER_H__

#include <unistd.h>
#include <stdexcept>
#include <ctime>
#include "CSV_message_queue.h"

using namespace std;

extern const unsigned char SIG_SHOW;
extern const unsigned char SIG_QUIT;
extern unsigned char SIG_STAT;


class CServer 
{
public:
	CServer();
	~CServer();

	void init(int argc, char** argv) throw(runtime_error, logic_error);
	void run();

	// SV-MQ operations
	void init_svmq() throw (runtime_error);
	//int enqueue(int iQueIndex, const void* vData, int iDataLen, int flag = IPC_NOWAIT);
	//int dequeue(int iQueIndex, void *vData, int& iDataLen, int iMaxDataLen, int flag = IPC_NOWAIT);
	int enqueue(const void* vData, int iDataLen, int flag = IPC_NOWAIT);
	int dequeue(void *vData, int iDataLen, int flag = IPC_NOWAIT);

	void clean_up();



private:
	CSVMessageQueue *m_pCSVMessageQueue;
	char m_szErrInfo[512];

	int m_iSendKey;
};

#endif
