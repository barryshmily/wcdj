#include "CServer.h"
#include "comm.h"


CServer::CServer()
{
	m_pCSVMessageQueue =  NULL;
	m_iSendKey         =  10088;// 0x2768

	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
}

CServer::~CServer()
{
	clean_up();
}

void CServer::clean_up()
{
	if (m_pCSVMessageQueue != NULL)
	{
		delete m_pCSVMessageQueue;
		m_pCSVMessageQueue = NULL;
	}
}

void CServer::init_svmq() throw (runtime_error)
{
	/*  create and init SV-MQ
	 *  echo 65535    > /proc/sys/kernel/msgmax, 64K
	 *  echo 16777216 > /proc/sys/kernel/msgmnb, 16M
	 *  so, msg_num = 256
	 *  */
	
	m_pCSVMessageQueue = new CSVMessageQueue();
	m_pCSVMessageQueue->init(m_iSendKey, IPC_FLAGS);// 0x272e

	int iRet = m_pCSVMessageQueue->create();
	if (iRet < 0) 
	{
		ERROR("CSVMessageQueue create err[%d:%s]", iRet, m_pCSVMessageQueue->get_err_info());
		throw runtime_error("CSVMessageQueue create err");
	}
	else if (1 == iRet)
	{
		INFO("CSVMessageQueue has existed\n");
		/* OK */
	}

	iRet = m_pCSVMessageQueue->open();
	if (iRet != 0) 
	{
		ERROR("CSVMessageQueue open err[%d:%s]", iRet, m_pCSVMessageQueue->get_err_info());
		throw runtime_error("CSVMessageQueue open err");
	}
}

void CServer::init(int argc, char** argv) throw(runtime_error, logic_error)
{

	// create and init SV-MQ
	init_svmq();

	INFO("CServer init ok!\n");
}

int CServer::enqueue(const void* vData, int iDataLen, int flag)
{

	if (iDataLen >= REQUSTMSG_MAX_LENGTH)
	{
		snprintf(m_szErrInfo, sizeof(m_szErrInfo), "enqueue: iDataLen[%d] >= REQUSTMSG_MAX_LENGTH[%d]", iDataLen, REQUSTMSG_MAX_LENGTH);
		return E_FAIL;
	}

	ur_msgbuf buf;
	buf.mtype   =  IPC_MQ_TYPE;
	buf.datalen =  iDataLen;
	memcpy(buf.mchar, vData, iDataLen);

	int iRet            =  E_OK;
	int iNum            =  0;
	const short TRY_NUM =  5;

	for (;;)
	{
		iRet = m_pCSVMessageQueue->send(buf, sizeof(ur_msgbuf) - sizeof(long), flag);
		if (iRet != E_OK)
		{
			// nonblock mode, sleep for a while and try some times
			if (flag == IPC_NOWAIT && iNum++ < TRY_NUM && errno == EAGAIN)
			{
				// sleep 1 ms
				struct timeval delay;
				delay.tv_sec  =  0;
				delay.tv_usec =  1 * 1000;
				select(0, NULL, NULL, NULL, &delay);

				continue;
			}
			else
			{
				// sorry, the SV-MQ can not be used!
				strcpy(m_szErrInfo, m_pCSVMessageQueue->get_err_info());
			}
		}

		break;
	}

	return iRet;
}

int CServer::dequeue(void *vData, int iDataLen, int flag)
{

	if (iDataLen < REQUSTMSG_MAX_LENGTH)
	{
		snprintf(m_szErrInfo, sizeof(m_szErrInfo), "dequeue: iDataLen[%d] < REQUSTMSG_MAX_LENGTH[%d]", iDataLen, REQUSTMSG_MAX_LENGTH);
		return E_FAIL;
	}

	ur_msgbuf buf;
	buf.mtype = IPC_MQ_TYPE;

	for (;;)
	{
		int iRet = m_pCSVMessageQueue->recv(buf, sizeof(ur_msgbuf) - sizeof(long), IPC_MQ_TYPE, flag);
		if (iRet != E_OK)
		{
			if (flag == IPC_NOWAIT && errno == ENOMSG)
			{
				continue;
			}
			else
			{
				strcpy(m_szErrInfo, m_pCSVMessageQueue->get_err_info());
				return iRet;
			}
		}

		break;
	}

	iDataLen = buf.datalen;
	memcpy(vData, buf.mchar, iDataLen);

	return 0;
}

/* loop job */
void CServer::run()
{

	srand((unsigned)time(NULL));

	for (;;)
	{
		try
		{
			if (SIG_STAT == SIG_SHOW)
			{
				// TODO
				INFO("get SIG_SHOW sig\n");
				SIG_STAT = 0;
			}
			else if (SIG_STAT == SIG_QUIT)
			{
				INFO("get SIG_QUIT sig, exit normal\n");
				exit(0);
			}

			// TODO

			// current time
			char szLocalTime[36] =  {0};
			time_t now           =  time(NULL);
			strftime(szLocalTime, sizeof(szLocalTime), "%Y%m%d-%H%M%S", localtime(&now));

#ifdef CLIENT_MODE
			INFO("client running...\n");

			// build request
			char szBuf[2048] = {0};
			snprintf(szBuf, sizeof(szBuf), "Hello, My name is gerryyang, time is %s", szLocalTime);

			int iRet = enqueue(szBuf, strlen(szBuf), IPC_NOWAIT);
			if (iRet != E_OK)
			{
				ERROR("send req to SV-MQ, key[%d] error[%s]\n", m_iSendKey, m_szErrInfo);
			}
			else
			{
				INFO("send req to SV-MQ, key[%d] ok!\n", m_iSendKey); 
			}

			// sleep 1001 ms
			struct timeval delay;
			delay.tv_sec  =  1;
			delay.tv_usec =  1 * 1000;
			select(0, NULL, NULL, NULL, &delay);

#else/* SERVER_MODE */
			INFO("server running...\n");

			// receive request
			char szBuf[REQUSTMSG_MAX_LENGTH] =  {0};
			int iDataLen                     =  sizeof(szBuf);

#if 0
			// block mode
			int iRet = dequeue(szBuf, iDataLen, 0);
			if (iRet != E_OK)
			{
				if (errno != EINTR)
				{
					ERROR("receive req from SV-MQ, key[%d] error[%s]\n", m_iSendKey, m_szErrInfo);
					break;

					/* fatal error, prog exit*/
					ERROR("server exit abnormally!\n");
				}

				// maybe interupted by signal that seems ok and go on working
				continue;
			}
#endif

			// non-block mode
			int iRet = dequeue(szBuf, iDataLen, IPC_NOWAIT);
			if (iRet != E_OK)
			{
				ERROR("receive req from SV-MQ, key[%d] error[%s]\n", m_iSendKey, m_szErrInfo);
			}
			else
			{
				INFO("receive req from SV-MQ, key[%d] ok! info[%s]\n", m_iSendKey, szBuf); 
			}
#endif

		}
		catch (std::bad_alloc &e)
		{
			ERROR("catch bad_alloc exception\n");
		}
		catch (...)
		{
			ERROR("catch unkown exception\n");
		}
	}
}


