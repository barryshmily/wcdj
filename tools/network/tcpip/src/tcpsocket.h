#ifndef __RTB_TCPSOCKET_H__
#define __RTB_TCPSOCKET_H__

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
using namespace std;

int tcp_send_portalinfo(const string &strMasterIP, unsigned short iMasterPort,
						const string &strSlaveIP, unsigned short iSlavePort,
						const char *buf, int len, string &strErr, int timeout);


#endif/* __RTB_TCPSOCKET_H__ */
