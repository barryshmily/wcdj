#include "CServer.h"
#include "Comm.h"

CServer::CServer()
{
	m_pCSVMessageQueueClient =  NULL;
	m_pCSVMessageQueueServer =  NULL;
	m_iClientSvMqKey         =  10088;// 0x2768
	m_iServerSvMqKey         =  10089;// 0x2769

	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
}

CServer::~CServer()
{
	clean_up();
}

void CServer::clean_up()
{
	if (m_pCSVMessageQueueClient != NULL) {
		delete m_pCSVMessageQueueClient;
		m_pCSVMessageQueueClient = NULL;
	}

	if (m_pCSVMessageQueueServer != NULL) {
		delete m_pCSVMessageQueueServer;
		m_pCSVMessageQueueServer = NULL;
	}
}

void CServer::init_svmq(CAppConfig &appconf_instance) throw (runtime_error)
{
	/*  create and init SV-MQ
	 *  echo 65535    > /proc/sys/kernel/msgmax, 64K
	 *  echo 16777216 > /proc/sys/kernel/msgmnb, 16M
	 *  so, min msg_num = 256
	 *  */

#ifdef CLIENT_MODE
	m_iClientSvMqKey = appconf_instance.get_clientsvmqkey();
#elif defined SERVER_MODE
	m_iServerSvMqKey = appconf_instance.get_serversvmqkey();
#elif defined PROXY_MODE
	m_iClientSvMqKey = appconf_instance.get_clientsvmqkey();
	m_iServerSvMqKey = appconf_instance.get_serversvmqkey();
#endif

	// create client and server SV-MQ
#ifdef CLIENT_MODE
	create_svmq(m_pCSVMessageQueueClient, m_iClientSvMqKey);
#elif defined SERVER_MODE
	create_svmq(m_pCSVMessageQueueServer, m_iServerSvMqKey);
#elif defined PROXY_MODE
	create_svmq(m_pCSVMessageQueueClient, m_iClientSvMqKey);
	create_svmq(m_pCSVMessageQueueServer, m_iServerSvMqKey);
#endif

	return;
}

void CServer::create_svmq(CSVMessageQueue * &pCSVMessageQueue, int iSvMqKey) throw (runtime_error)
{
	pCSVMessageQueue = new CSVMessageQueue();
	pCSVMessageQueue->init(iSvMqKey, IPC_FLAGS);

	int iRet = pCSVMessageQueue->create();
	if (iRet < 0) {
		ERROR("CSVMessageQueue create err[%d:%s]", iRet, pCSVMessageQueue->get_err_info());
		throw runtime_error("CSVMessageQueue create err");

	} else if (1 == iRet) {
		INFO("CSVMessageQueue has existed\n");
		/* OK */
	}

	iRet = pCSVMessageQueue->open();
	if (iRet != 0) {
		ERROR("CSVMessageQueue open err[%d:%s]", iRet, pCSVMessageQueue->get_err_info());
		throw runtime_error("CSVMessageQueue open err");
	}

	return;
}

void CServer::init(CAppConfig &appconf_instance) throw(runtime_error, logic_error)
{
	// create and init SV-MQ
	init_svmq(appconf_instance);

	INFO("CServer init ok!\n");
}

int CServer::enqueue(CSVMessageQueue * &pCSVMessageQueue, const void* vData, int iDataLen, int flag)
{

	if (iDataLen >= REQUSTMSG_MAX_LENGTH) {
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

	for (;;) {
		iRet = pCSVMessageQueue->send(buf, sizeof(ur_msgbuf) - sizeof(long), flag);
		if (iRet != E_OK) {
			// nonblock mode, sleep for a while and try some times
			if (flag == IPC_NOWAIT && iNum++ < TRY_NUM && errno == EAGAIN) {
				// sleep 1 ms
				struct timeval delay;
				delay.tv_sec  =  0;
				delay.tv_usec =  1 * 1000;
				select(0, NULL, NULL, NULL, &delay);

				continue;

			} else {
				// sorry, the SV-MQ can not be used!
				strcpy(m_szErrInfo, pCSVMessageQueue->get_err_info());
			}
		}
		break;
	}

	return iRet;
}

int CServer::dequeue(CSVMessageQueue * &pCSVMessageQueue, void *vData, int iDataLen, int flag)
{

	if (iDataLen < REQUSTMSG_MAX_LENGTH) {
		snprintf(m_szErrInfo, sizeof(m_szErrInfo), "dequeue: iDataLen[%d] < REQUSTMSG_MAX_LENGTH[%d]", iDataLen, REQUSTMSG_MAX_LENGTH);
		return E_FAIL;
	}

	ur_msgbuf buf;
	buf.mtype = IPC_MQ_TYPE;

	for (;;) {
		int iRet = pCSVMessageQueue->recv(buf, sizeof(ur_msgbuf) - sizeof(long), IPC_MQ_TYPE, flag);
		if (iRet != E_OK) {
			if (flag == IPC_NOWAIT && errno == ENOMSG) {
				continue;

			} else {
				strcpy(m_szErrInfo, pCSVMessageQueue->get_err_info());
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

	for (;;) {

		try {
			if (SIG_STAT == SIG_SHOW) {
				// TODO
				INFO("get SIG_SHOW sig\n");
				SIG_STAT = 0;

			} else if (SIG_STAT == SIG_QUIT) {
				INFO("get SIG_QUIT sig, exit normal\n");
				exit(0);
			}

			// TODO

			// current time
			char szLocalTime[36] =  {0};
			time_t now           =  time(NULL);
			//strftime(szLocalTime, sizeof(szLocalTime), "%Y%m%d-%H%M%S", localtime(&now));

#ifdef CLIENT_MODE
			//INFO("client running...\n");

			// build request
			char szBuf[2048] = {0};
			//snprintf(szBuf, sizeof(szBuf), "Hello, My name is gerryyang, time is %s", szLocalTime);
			snprintf(szBuf, sizeof(szBuf), "%d", (int)now);

			static int iReqNum    =  0;
			static int iCurrent   =  0;
			static bool bFirst    =  true;
			static int iPrintFreq =  0;

			int iRet = enqueue(m_pCSVMessageQueueClient, szBuf, strlen(szBuf), IPC_NOWAIT);
			if (iRet != E_OK) {
				ERROR("[time=%d]send req to SV-MQ, key[%d] error[%s]\n", (int)now, m_iClientSvMqKey, m_szErrInfo);
				__LOG("[time=%d]send req to SV-MQ, key[%d] error[%s]\n", (int)now, m_iClientSvMqKey, m_szErrInfo);

			} else {
				//INFO("send req to SV-MQ, key[%d] ok! req[%s] \n", m_iClientSvMqKey, szBuf);

				// calc the number of every second client could send requset
				int iSend =  now;
				if (bFirst) {
					iCurrent =  iSend;
					bFirst   =  false;
				}

				if (iCurrent == iSend) {
					++iReqNum >= 2147483647 ? iReqNum = 0 : NULL;

				} else {
					if (++iPrintFreq % 10 == 0) {
						INFO("send req to SV-MQ, key[%d] ok! time[%d] reqnum[%d] req[%d]\n",
						     m_iClientSvMqKey, iCurrent, iReqNum, iSend - 1);
						iPrintFreq = 0;
					}

					// reset
					bFirst  =  true;
					iReqNum =  1;
				}

			}

#if 1
			// sleep 1 us
			struct timeval delay;
			delay.tv_sec  =  0;
			delay.tv_usec =  1 * 1;
			select(0, NULL, NULL, NULL, &delay);
#endif

#elif defined SERVER_MODE
			//INFO("server running...\n");

			// receive request
			char szBuf[REQUSTMSG_MAX_LENGTH] =  {0};
			int iDataLen                     =  sizeof(szBuf);


			static int iReqNum    =  0;
			static int iCurrent   =  0;
			static bool bFirst    =  true;
			static int iPrintFreq =  0;


#ifdef BLOCK_ACCEPT
			/* block mode
			 * If (msgflg & IPC_NOWAIT) is 0, the calling thread will suspend execution until one of the following occurs:
			 * [1] A message of the desired type is placed on the queue.
			 * [2] The message queue identifier msqid is removed from the system; when this occurs, errno is set equal to [EIDRM] and -1 is returned.
			 * [3] The calling thread receives a signal that is to be caught; in this case a message is not received and the calling thread resumes execution in the manner prescribed in sigaction().
			 * */
			int iRet = dequeue(m_pCSVMessageQueueServer, szBuf, iDataLen, 0);
			if (iRet != E_OK) {
				if (errno != EINTR) {
					ERROR("receive req from SV-MQ, key[%d] error[%s]\n", m_iServerSvMqKey, m_szErrInfo);
					__LOG("receive req from SV-MQ, key[%d] error[%s]\n", m_iServerSvMqKey, m_szErrInfo);
					/* fatal error, prog exit*/
					break;
				}

				// maybe interupted by signal that seems ok and go on working
				continue;

			} else {
				//INFO("receive req from SV-MQ, key[%d] ok! info[%s]\n", m_iServerSvMqKey, szBuf);

				// calc the number of every second svr could accept requset
				int iAccept =  atoi(szBuf);
				if (bFirst) {
					iCurrent =  iAccept;
					bFirst   =  false;
				}

				if (iCurrent == iAccept) {
					++iReqNum >= 2147483647 ? iReqNum = 0 : NULL;

				} else {
					if (++iPrintFreq % 10 == 0) {
						INFO("receive req from SV-MQ, key[%d] ok! time[%d] reqnum[%d] req[%d]\n",
						     m_iServerSvMqKey, iCurrent, iReqNum, iAccept - 1);
						iPrintFreq = 0;
					}

					// reset
					bFirst  =  true;
					iReqNum =  1;
				}
			}
#else
			// non-block mode, Note: this will cause CPU usage too high, so don't use this usually!
			int iRet = dequeue(m_pCSVMessageQueueServer, szBuf, iDataLen, IPC_NOWAIT);
			if (iRet != E_OK) {
				ERROR("receive req from SV-MQ, key[%d] error[%s]\n", m_iServerSvMqKey, m_szErrInfo);

			} else {
				INFO("receive req from SV-MQ, key[%d] ok! info[%s]\n", m_iServerSvMqKey, szBuf);
			}
#endif

#elif defined PROXY_MODE

			//INFO("proxy running...\n");

			// receive request
			char szBuf[REQUSTMSG_MAX_LENGTH] =  {0};
			int iDataLen                     =  sizeof(szBuf);


			static int iReqNum    =  0;
			static int iCurrent   =  0;
			static bool bFirst    =  true;
			static int iPrintFreq =  0;


#ifdef BLOCK_ACCEPT
			/* block mode
			 * If (msgflg & IPC_NOWAIT) is 0, the calling thread will suspend execution until one of the following occurs:
			 * [1] A message of the desired type is placed on the queue.
			 * [2] The message queue identifier msqid is removed from the system; when this occurs, errno is set equal to [EIDRM] and -1 is returned.
			 * [3] The calling thread receives a signal that is to be caught; in this case a message is not received and the calling thread resumes execution in the manner prescribed in sigaction().
			 * */
			int iRet = dequeue(m_pCSVMessageQueueClient, szBuf, iDataLen, 0);
			if (iRet != E_OK) {
				if (errno != EINTR) {
					ERROR("receive req from SV-MQ, key[%d] error[%s]\n", m_iServerSvMqKey, m_szErrInfo);
					__LOG("receive req from SV-MQ, key[%d] error[%s]\n", m_iServerSvMqKey, m_szErrInfo);
					/* fatal error, prog exit*/
					break;
				}

				// maybe interupted by signal that seems ok and go on working
				continue;

			} else {
				//INFO("receive req from SV-MQ, key[%d] ok! info[%s]\n", m_iServerSvMqKey, szBuf);

				// calc the number of every second proxy could accept requset
				int iAccept =  atoi(szBuf);
				if (bFirst) {
					iCurrent =  iAccept;
					bFirst   =  false;
				}

				if (iCurrent == iAccept) {
					++iReqNum >= 2147483647 ? iReqNum = 0 : NULL;

				} else {
					if (++iPrintFreq % 10 == 0) {
						INFO("receive req from SV-MQ, key[%d] ok! time[%d] reqnum[%d] req[%d]\n",
						     m_iServerSvMqKey, iCurrent, iReqNum, iAccept - 1);
						iPrintFreq = 0;
					}

					// reset
					bFirst  =  true;
					iReqNum =  1;
				}
			}
#else
			// non-block mode, Note: this will cause CPU usage too high, so don't use this usually!
			int iRet = dequeue(m_pCSVMessageQueueClient, szBuf, iDataLen, IPC_NOWAIT);
			if (iRet != E_OK) {
				ERROR("receive req from SV-MQ, key[%d] error[%s]\n", m_iServerSvMqKey, m_szErrInfo);

			} else {
				INFO("receive req from SV-MQ, key[%d] ok! info[%s]\n", m_iServerSvMqKey, szBuf);
			}
#endif

			//  repost request to back-end svr

			iRet = enqueue(m_pCSVMessageQueueServer, szBuf, strlen(szBuf), IPC_NOWAIT);
			if (iRet != E_OK) {
				ERROR("[time=%d]send req to SV-MQ, key[%d] error[%s]\n", (int)now, m_iClientSvMqKey, m_szErrInfo);
				__LOG("[time=%d]send req to SV-MQ, key[%d] error[%s]\n", (int)now, m_iClientSvMqKey, m_szErrInfo);

			} else {
				//INFO("send req to SV-MQ, key[%d] ok! req[%s] \n", m_iClientSvMqKey, szBuf);
			}

#if 0
			// sleep 1001 ms
			struct timeval delay;
			delay.tv_sec  =  1;
			delay.tv_usec =  1 * 1000;
			select(0, NULL, NULL, NULL, &delay);
#endif


#endif/*mode end*/

		} catch (std::bad_alloc &e) {
			ERROR("catch bad_alloc exception\n");

		} catch (...) {
			ERROR("catch unkown exception\n");
		}
	}
}


