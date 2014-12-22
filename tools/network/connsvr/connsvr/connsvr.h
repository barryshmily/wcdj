#ifndef CONNSVR_H_H
#define CONNSVR_H_H

#include <sys/resource.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "event.h"
#include "anet.h"
#include "Log.h"

#define MAX_CLIENT_COUNT 50000          //每个进程最多处理客户数量
#define MAX_BUF_SIZE     4096
#define MAX_TIMEOUT      60000          //最大超时时间

typedef struct conn
{
    struct event_base* base;
    struct time_event* tevent;      /*对应超时事件*/

    int fd;
    int plen;                       /*包长度*/

    char* rbuf;                     /*存储接收数据*/
    int   rtotal;                   /*接收Buffer总长度*/
    int   rlen;                     /*接收数据长度*/

    char* sbuf;                     /*存储发送数据*/
    int   stotal;                   /*发送Buffer总长度*/
    int   slen;                     /*发送数据长度*/
    int   spos;                     /*已发送数据位置*/

    char ip[64];
    int port;
} conn;

void append_pkt(struct conn* c, int len, char* buf=NULL);
char* resize_rbuf(struct conn* c, int len);
char* resize_sbuf(struct conn* c, int len);
struct event_base* event_base_init(int fd);
int create_server(const char* ip, int port);

#endif

