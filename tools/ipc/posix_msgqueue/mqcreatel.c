/* 
 * 2012-05-27 wcdj 
 * UNP2 P.59 
 * Posix 消息队列 
*/  
  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/stat.h>  
#include <mqueue.h>  
#include <string.h>  
#include <errno.h>  
#include <signal.h>  
   
/* default permissions for new files */  
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)  
  
/* global var */  
mqd_t g_mqd;  
struct mq_attr g_attr;  
struct sigevent g_sigev;  
void *g_buff;  
  
static void sig_usr1(int);  
  
void err_quit(char *pErrInfo)  
{  
    printf("%s\n", pErrInfo);  
    exit(1);  
}  
  
void help()  
{  
    err_quit("\nUSAGE: \n"  
            "create:   posix_mq -e <name> [-m maxmsg -z msgsize]\n"  
            "delete:   posix_mq -d <name>\n"  
            "info:     posix_mq -i <name>\n"  
            "send:     posix_mq -s <name> <#byte> <priority>\n"  
            "read:     posix_mq -r <name> -n <0/1>\n"  
            "limit:    posix_mq -l\n"  
            "notify:   posix_mq -y <name>\n"  
  
            "\nFor example, input ./posix_mq -i /wcdj\n");  
}  
  
  
int   
main(int argc, char **argv)  
{  
    int c, flags;  
    mqd_t mqd;  
    flags = O_RDWR;  
    char szName[1024] = {0};  
    struct mq_attr attr;  
    memset(&attr, 0, sizeof(attr));  
  
    // mark used  
    bool bOpt_i = false;  
    bool bOpt_m = false;  
    bool bOpt_s = false;  
    bool bOpt_r = false;  
    bool bOpt_n = false;  
    bool bOpt_l = false;  
    bool bOpt_y = false;  
  
    if (argc == 1)  
    {  
        help();   
    }  
  
      
    while ((c = getopt(argc, argv, "e:d:i:hm:z:s:r:n:ly:")) != -1)  
    {  
        switch (c)  
        {  
            // create  
            case 'e':  
                    flags |= O_CREAT | O_EXCL;  
                    snprintf(szName, sizeof(szName), "%s", optarg);  
                    break;  
  
            // delete  
            case 'd':  
                    snprintf(szName, sizeof(szName), "%s", optarg);  
                    mq_unlink(szName);  
                    printf("mq_unlink(): success\n");  
                    return 0;  
                    break;  
  
            // info  
            case 'i':  
                    snprintf(szName, sizeof(szName), "%s", optarg);  
                    bOpt_i = true;  
                    break;  
  
            // set attr  
            case 'm':  
                    attr.mq_maxmsg = atol(optarg);  
                    bOpt_m = true;  
                    break;  
            case 'z':  
                    attr.mq_msgsize = atol(optarg);  
                    break;  
  
            // send  
            case 's':  
                    snprintf(szName, sizeof(szName), "%s", optarg);  
                    bOpt_s = true;    
                    break;  
  
            // read  
            case 'r':  
                    snprintf(szName, sizeof(szName), "%s", optarg);  
                    bOpt_r = true;    
                    break;  
            case 'n':  
                    optarg > 0 ? bOpt_n = true : bOpt_n = false;  
                    break;  
  
            // limit   
            case 'l':  
                    bOpt_l = true;  
                    break;  
  
            // notify  
            case 'y':  
                    snprintf(szName, sizeof(szName), "%s", optarg);  
                    bOpt_y = true;    
                    break;  
                      
  
            case 'h':  
            default:  
                    help();  
                  
        }  
    }  
    //printf("optind = %d\n", optind);  
    //printf("opterr = %d\n", opterr);  
  
    // send  
    if (bOpt_s)  
    {  
        if (argc != 5) help();  
        int ibytes = atoi(argv[optind]);  
        int ipriority = atoi(argv[++optind]);  
        mqd = mq_open(szName, O_WRONLY);  
        char *ptr = "Hi, wcdj is a guy!";  
        int iret = mq_send(mqd, ptr, ibytes, ipriority);  
        if (iret == 0)  
            printf("send succ: %s bytes=%d priority=%d\n", szName, ibytes, ipriority);  
        else  
            printf("send err: %s bytes=%d priority=%d\n", szName, ibytes, ipriority);  
  
        mq_getattr(mqd, &attr);  
        printf("max msgs = %ld, max bytes/msg = %ld, currently on queue = %ld\n", \  
                attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);  
  
        return 0;  
    }  
  
    // read  
    if (bOpt_r)  
    {  
        void *buff;  
        ssize_t n;  
        unsigned prio;  
        struct mq_attr attr;  
  
        if (bOpt_n)// non-block read  
        {  
            mqd = mq_open(szName, O_RDONLY | O_NONBLOCK);  
            mq_getattr(mqd, &attr);  
            buff = malloc(attr.mq_msgsize);  
            n = mq_receive(mqd, (char *)buff, attr.mq_msgsize, &prio);  
            if (n != -1)  
                printf("non-block read %ld bytes, priority=%u, buff=%s\n", (long)n, prio, (char *)buff);  
            else  
                printf("non-block read error: %s\n", strerror(errno));  
        }  
        else// block read  
        {  
            mqd = mq_open(szName, O_RDONLY);  
            mq_getattr(mqd, &attr);  
            buff = malloc(attr.mq_msgsize);  
            n = mq_receive(mqd, (char *)buff, attr.mq_msgsize, &prio);  
            if (n != -1)  
                printf("block read %ld bytes, priority=%u, buff=%s\n", (long)n, prio, (char *)buff);  
            else  
                printf("block read error\n");  
        }  
  
        return 0;  
    }  
  
    // limit  
    if (bOpt_l)  
    {  
        // find /usr/include -name "unistd.h" | xargs egrep "*MAX" -rn  
        printf("MQ_OPEN_MAX = %ld, MQ_PRIO_MAX = %ld\n",  
            sysconf(_SC_MQ_OPEN_MAX), sysconf(_SC_MQ_PRIO_MAX));  
        return 0;  
    }  
  
    // notify  
    if (bOpt_y)  
    {  
  
        // open queue, get attributes, allocate read buffer  
        g_mqd = mq_open(szName, O_RDONLY);  
        mq_getattr(g_mqd, &g_attr);  
        g_buff =    malloc(g_attr.mq_msgsize);  
  
        // establish signal handler, enable notification  
        signal(SIGUSR1, sig_usr1);  
        g_sigev.sigev_notify = SIGEV_SIGNAL;  
        g_sigev.sigev_signo = SIGUSR1;  
        int iret = mq_notify(g_mqd, &g_sigev);  
        if (iret != 0)  
        {  
            printf("mq_notify error: %s\n", strerror(errno));  
            return -1;  
        }  
  
        for (;;)  
            pause();// signal handler does everything  
  
        return 0;  
  
    }  
  
  
  
    if ((attr.mq_maxmsg != 0 && attr.mq_msgsize == 0)   
        || (attr.mq_maxmsg == 0 && attr.mq_msgsize != 0))  
    {  
        err_quit("you must specify both -m maxmsg and -z msgsize!");  
    }  
    if (strcmp(szName, "") == 0)  
    {  
        help();  
    }  
  
    mqd = mq_open(szName, flags, FILE_MODE, (attr.mq_maxmsg != 0) ? &attr : NULL);  
    if ((mqd_t)-1 == mqd)  
    {  
        perror("mq_open()");  
        exit(1);  
    }  
    else  
        printf("mq_open(): success\n");  
          
    if (bOpt_i || bOpt_m)  
    {  
        mq_getattr(mqd, &attr);  
        printf("max msgs = %ld, max bytes/msg = %ld, currently on queue = %ld\n", \  
                attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);  
    }  
  
    mq_close(mqd);  
  
    return 0;  
}  
  
static void  
sig_usr1(int signo)  
{  
    ssize_t n;  
  
    // Re-register first  
    mq_notify(g_mqd, &g_sigev);  
  
    n = mq_receive(g_mqd, (char *)g_buff, g_attr.mq_msgsize, NULL);  
    printf("SIGUSR1 received, read %ld bytes\n", (long)n);  
      
    return;  
}  
  
  
/* 
g++ -Wall -g -lrt -o posix_mq mqcreatel.c 
 
test: 
 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -d /wcdj         
mq_unlink(): success 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -i /wcdj  
mq_open(): No such file or directory 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -e /wcdj -m 2 -z 5 
mq_open(): success 
max msgs = 2, max bytes/msg = 5, currently on queue = 0 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -r /wcdj 
 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -r /wcdj -n 1 
non-block read error: Resource temporarily unavailable 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -s /wcdj 10 20 
send err: /wcdj bytes=10 priority=20 
max msgs = 2, max bytes/msg = 5, currently on queue = 0 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -s /wcdj 5 20  
send succ: /wcdj bytes=5 priority=20 
max msgs = 2, max bytes/msg = 5, currently on queue = 1 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -r /wcdj  
block read 5 bytes, priority=20, buff=Hi, w 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -e /wcdj -m 2 -z 10 
mq_open(): File exists 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -d /wcdj             
mq_unlink(): success 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -e /wcdj -m 2 -z 10 
mq_open(): success 
max msgs = 2, max bytes/msg = 10, currently on queue = 0 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -s /wcdj 10 1 
send succ: /wcdj bytes=10 priority=1 
max msgs = 2, max bytes/msg = 10, currently on queue = 1 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -s /wcdj 10 2 
send succ: /wcdj bytes=10 priority=2 
max msgs = 2, max bytes/msg = 10, currently on queue = 2 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -s /wcdj 10 3 
 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -i /wcdj   
mq_open(): success 
max msgs = 2, max bytes/msg = 10, currently on queue = 2 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -r /wcdj   
block read 10 bytes, priority=2, buff=Hi, wcdj i 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -r /wcdj  
block read 10 bytes, priority=1, buff=Hi, wcdj i 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -r /wcdj  
 
gerryyang@wcdj:~/test/code_in_action/UNP2> ./posix_mq -r /wcdj -n 1 
non-block read error: Resource temporarily unavailable 
gerryyang@wcdj:~/test/code_in_action/UNP2>  
 
*/  
