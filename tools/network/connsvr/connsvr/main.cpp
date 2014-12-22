/** Copyright(C)  1998-2014,Tencent Co. Ltd.
* File name:    main.cpp
* @author:      jiwubu
* @version:     V1.0   2014/09/14
* Description: 	socket多进程实现,监控子进程
* Log:
*/

#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "process.h"
#include "connsvr.h"

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

void handler(int sig)
{
    Err("Main Process PID=%d recv SIGALRM signal, notify child process exit", getpid() );

    kill(0, SIGTERM);

    exit(0);
}

void child_handler(int sig)
{
    Err("Child Process PID=%d recv SIGALRM signal, exit", getpid() );

    exit(0);
}

int main(int argc, char* argv[])
{
    if( argc < 4 )
    {
        printf("./connsvr listenip listenport process_num\n");

        return -1;
    }

    const char* szServerIP = argv[1];
    int iServerPort = atoi(argv[2]);
    int num_childs = atoi(argv[3]);

    if( num_childs <= 0 )
    {
        num_childs = 1;
    }

    init_daemon();

    signal(SIGCHLD, SIG_DFL);
    signal(SIGTERM, handler);

    G_pLog = CreateLog("connsvr.log", 5, 1024 * 10240, 5);

    if ( NULL == G_pLog )
    {
        fprintf(stderr, "Can't create log file\n");

        return -1;
    }

    int fd = create_server(szServerIP, iServerPort);

    //创建server
    if( fd < 0 )
    {
        Err("Can server [%s:%d] failed", szServerIP, iServerPort );

        return -1;
    }

    int child = 0;
    int iret = 0;

    while ( !child )
    {
        if ( num_childs > 0)
        {
            iret = fork();

            switch(iret)
            {
                case -1:
                    Err("fork error, %s", strerror(errno) );
                    return -1;

                case 0:
                    child = 1;
                    break;

                default:
                    Info("create child process pid=%d ok", iret );
                    num_childs--;
                    break;
            }
        }
        else
        {
            int status;

            int pid = wait(&status);

            if ( pid != -1 )
            {
                num_childs++;
                Err("moinitor process pid =%d exit", pid);
            }
            else
            {
                Err("wait return error, %s", strerror(errno) );
                break;
            }
        }
    }

    if (!child)
    {
        Info("Main Process exit");

        kill(0, SIGTERM);

        return 0;
    }

    DestroyLog(G_pLog);

    char szLogName[1024] = {0};

    snprintf(szLogName, sizeof(szLogName) - 1, "connsvr-%d", getpid() );

    //在子进程中重新创建日志句柄,防止多进程同时写同一日志与滚动
    G_pLog = CreateLog(szLogName, 5, 1024 * 1024 * 10, 5);

    if ( NULL == G_pLog )
    {
        fprintf(stderr, "Can't create log file\n");

        return -1;
    }

    //注册SIGTERM信号的回调函数,以便接收父进程退出时的信号
    signal(SIGTERM, child_handler);

    Info("create child process pid=%d", getpid());

    struct event_base* base = event_base_init(fd);

    if( NULL == base )
    {
        Err("Can't create file_event loop: %s", strerror(errno) );

        return -1;
    }

    event_dispatch(base);

    return 0;
}

