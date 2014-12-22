#ifndef PROCESS_H_H
#define PROCESS_H_H

#include "connsvr.h"
#include "package.h"

int process_packet(struct conn* c, char* buf, int len);
void send_client(struct conn* c, struct pbpack* pkt);
void write_stat(const char* reqIp, const char* userIp, unsigned int ulQQ,
                const char* domain, const char* pszData);

/* 统计服务器fd */
extern int G_stat_fd;

/* 统计服务器sa */
extern struct sockaddr_in G_stat_sa;

#endif

