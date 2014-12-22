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

#define MAX_CLIENT_COUNT 80000          //每个进程最多处理客户数量
#define MAX_BUF_SIZE     2048

typedef struct conn
{
    struct event_base* base;

    int fd;
    struct conn *remote;

    char buf[MAX_BUF_SIZE];

    int total;
    int wpos;
    int rpos;

    char ip[32];
    int port;

    void (*OnError)(struct conn *c);
} conn;

struct event_base* event_base_init(int fd);
int create_server(const char* ip, int port);

extern char G_ProxyIP[64];
extern int G_ProxyPort;

#endif

