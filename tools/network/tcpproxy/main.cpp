/** Copyright(C)  1998-2014,Tencent Co. Ltd.
* File name:    main.cpp
* @author:      jiwubu
* @version:     V1.0   2014/09/14
* Description: 	tcpproxy
* Log:
*/

#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "connsvr.h"

char G_ProxyIP[64] = {0};
int G_ProxyPort = 0;

//后台执行
void init_daemon()
{
    rlimit rlim,rlim_new;

    if (getrlimit(RLIMIT_NOFILE, &rlim)==0)
    {
        rlim_new.rlim_cur = rlim_new.rlim_max = 200000;

        if (setrlimit(RLIMIT_NOFILE, &rlim_new)!=0)
        {
            Warn("%s","[Main]:Setrlimit file Fail, use old rlimit!\n");

            rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
            setrlimit(RLIMIT_NOFILE, &rlim_new);
        }
    }

    if (getrlimit(RLIMIT_CORE, &rlim)==0)
    {
        rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;

        if (setrlimit(RLIMIT_CORE, &rlim_new)!=0)
        {
            Warn("%s","[Main]:Setrlimit core Fail, use old rlimit!\n");

            rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
            setrlimit(RLIMIT_CORE, &rlim_new);
        }
    }

    signal(SIGPIPE, SIG_IGN);
    pid_t pid;

    if ((pid = fork() ) != 0 )
    {
        exit( 0);
    }

    setsid();
    signal( SIGINT,  SIG_IGN);
    signal( SIGHUP,  SIG_IGN);
    signal( SIGQUIT, SIG_IGN);
    signal( SIGPIPE, SIG_IGN);
    signal( SIGTTOU, SIG_IGN);
    signal( SIGTTIN, SIG_IGN);
    signal( SIGCHLD, SIG_IGN);
    signal( SIGTERM, SIG_IGN);

    struct sigaction sig;
    sig.sa_handler = SIG_IGN;
    sig.sa_flags = 0;
    sigemptyset( &sig.sa_mask);
    sigaction( SIGHUP,&sig,NULL);

    if ((pid = fork() ) != 0 )
    {
        exit(0);
    }

    umask(0);
    setpgrp();

    return;
}

int main(int argc, char* argv[])
{
    if( argc < 4 )
    {
        printf("./tcpproxy listenip listenport proxyip proxyport\n");

        return -1;
    }

    const char* szServerIP = argv[1];
    int iServerPort = atoi(argv[2]);

    snprintf(G_ProxyIP, sizeof(G_ProxyIP) - 1, "%s", argv[3]);
    G_ProxyPort = atoi(argv[4]);

    //设置为后台启动
    init_daemon();

    //初始化创建日志句柄
    G_pLog = CreateLog("tcpproxy.log", 5, 1024 * 10240, 5);

    if ( NULL == G_pLog )
    {
        fprintf(stderr, "Can't create log file\n");

        return -1;
    }

    //创建监听服务器
    int fd = create_server(szServerIP, iServerPort);

    if( fd < 0 )
    {
        Err("Can server [%s:%d] failed", szServerIP, iServerPort );

        return -1;
    }

    //初始化事件框架，并将listenfd注册到事件框架中
    struct event_base* base = event_base_init(fd);

    if( NULL == base )
    {
        Err("Can't create file_event loop: %s", strerror(errno) );

        return -1;
    }

    //事件驱动循环
    event_dispatch(base);

    return 0;
}

