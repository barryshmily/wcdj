#include "tcpsocket.h"

namespace RTBNet
{

	int tcp_send_portalinfo(const string &strMasterIP, unsigned short iMasterPort,
			const string &strSlaveIP, unsigned short iSlavePort,
			const char *buf, int len, string &strErr, int timeout)
	{

		int iSock = socket(AF_INET, SOCK_STREAM, 0);
		if (iSock < 0)
		{
			strErr = "tcp_send_portalinfo: socket err[" + string(strerror(errno)) + "]";
			return -1;
		}

		struct timeval timeo =  {0, 0};
		socklen_t tolen      =  sizeof(timeo);
		timeo.tv_usec        =  timeout * 1000;

		// 设置连接超时选项
		if ( 0 != setsockopt(iSock, SOL_SOCKET, SO_SNDTIMEO, &timeo, tolen))
		{
			strErr = "tcp_send_portalinfo: setsockopt err[" + string(strerror(errno)) + "]";
			close(iSock);
			return -1;
		}

		struct sockaddr_in stSvr;
		memset(&stSvr, 0x0, sizeof(stSvr));
		stSvr.sin_family = AF_INET;
		struct in_addr in_val;

		if (!inet_aton(strMasterIP.c_str(), &in_val))
		{
			strErr = "tcp_send_portalinfo: inet_aton err[" + string(strerror(errno)) + "]";
			close(iSock);
			return -1;
		}
		memcpy((void*)&(stSvr.sin_addr), (void*)&in_val, sizeof(struct in_addr));
		stSvr.sin_port = htons(iMasterPort);

		/* 
		 * 先连主后连备, 主备均连不上则返回失败
		 * */

		// 连主
		if (0 != connect(iSock, (struct sockaddr*)&stSvr, sizeof(stSvr)))
		{
			// 如果连接失败需关闭之前套接字, gerryyang
			close(iSock);

			// conntect to server-2
			iSock = socket(AF_INET, SOCK_STREAM, 0);
			if (iSock < 0)
			{
				strErr = "tcp_send_portalinfo: socket2 err[" + string(strerror(errno)) + "]";
				return -1;
			}

			struct timeval timeo =  {0, 0};
			socklen_t tolen      =  sizeof(timeo);
			timeo.tv_usec        =  timeout;

			if (0 != setsockopt(iSock, SOL_SOCKET, SO_SNDTIMEO, &timeo, tolen))
			{
				strErr = "tcp_send_portalinfo: setsockopt2 err[" + string(strerror(errno)) + "]";
				close(iSock);
				return -1;
			}

			// try server-2
			memset(&stSvr, 0x0, sizeof(stSvr));
			stSvr.sin_family = AF_INET;
			if (!inet_aton(strSlaveIP.c_str(), &in_val))
			{
				strErr = "tcp_send_portalinfo: inet_aton2 err[" + string(strerror(errno)) + "]";
				close(iSock);
				return -1;
			}
			memcpy((void*)&(stSvr.sin_addr), (void*)&in_val, sizeof(struct in_addr));
			stSvr.sin_port = htons(iSlavePort);

			// 连备
			if (connect(iSock, (struct sockaddr*)&stSvr, sizeof(stSvr)))
			{
				// give up
				char szErr[1024] = {0};
				snprintf(szErr, sizeof(szErr), "tcp_send_portalinfo: connect to master[%s:%d] and slave[%s:%d] both err[%s]",
						strMasterIP.c_str(), iMasterPort, strSlaveIP.c_str(), iSlavePort, strerror(errno));
				strErr = szErr;
				close(iSock);
				return -1;
			}
		}

		// send request
		const char *p =  buf;
		size_t nLeft  =  len;
		ssize_t nSent =  0;

		while (nLeft > 0)
		{
			// send() returns the number of bytes actually sent out, this might be less than the number you told it to send
			nSent = send(iSock, p, nLeft, 0);
			if (nSent <= 0)
			{
				if (nSent < 0 && errno == EINTR)
					continue;// continue writing
				else
					break;
			}
			nLeft -= nSent;
			p += nSent;
		}

		if (nSent <= 0)
		{
			close(iSock);
			strErr = "tcp_send_portalinfo: send err[" + string(strerror(errno)) + "]";
			return -1;
		}

		// 不需要接收应答
		close(iSock);

#if 0
		// waiting for response
		int iRet = 0;
		memset(buf, 0x0, len);
		p = buf;
		fd_set stReadFds;
		struct timeval stTimeVal;
		FD_ZERO(&stReadFds);
		FD_SET(iSock, &(stReadFds));

		while (1)
		{
			stTimeVal.tv_sec  =  3;
			stTimeVal.tv_usec =  0;

			iRet = select(iSock + 1, &stReadFds, NULL, NULL, &stTimeVal);
			if (iRet <= 0)
			{
				close(iSock);
				strErr = "tcp_send_portalinfo: select err[" + string(strerror(errno)) + "]";
				return -1;
			}

			iRet = recv(iSock, p, len, MSG_DONTWAIT);
			if (iRet < 0)
			{
				close(iSock);
				strErr = "tcp_send_portalinfo: recv err[" + string(strerror(errno)) + "]";
				return -1;
			}

			// connection closed
			if (iRet == 0)
				break;

			p += iRet;
		}
		close(iSock);

		// parse answer
		// TODO

#endif

		return 0;
	}

}
