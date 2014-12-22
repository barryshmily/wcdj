#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/epoll.h>
#include "package.h"

int SetNoBlock(int fd)
{
    int val = fcntl(fd, F_GETFL, 0);

    if ( val == -1)
    {
        return -1;
    }

    if (fcntl(fd, F_SETFL, val | O_NONBLOCK | O_NDELAY) == -1)
    {
        return -1;
    }

    return 0;
}

int Select(int fd, int timeout, int event)
{
    struct epoll_event events[10];
    int ret = -1;
    int flag = 1;

    int epfd = epoll_create(10);

    if( epfd < 0 )
    {
        printf("Can't epoll_create: %s\n", strerror(errno));

        return -1;
    }

    struct epoll_event ev;

    ev.data.fd = fd;
    ev.events = event;

    if( epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) < 0 )
    {
        printf("epoll_ctl error: %s\n", strerror(errno));

        goto err;
    }

    while( flag )
    {
        int nfds = epoll_wait(epfd, events, 10, 1000 * timeout);

        if( nfds == 0 )
        {
            ret = 0;

            break;
        }
        else if( nfds < 0 && errno != EINTR)
        {
            printf("epoll_wait error: %s\n", strerror(errno) );

            goto err;
        }

        for(int i = 0; i < nfds; ++i)
        {
            if ( events[i].events & event )
            {
                ret = 1;

                flag = 0;
            }
        }
    }

err:
    close(epfd);

    return ret;
}

int Open(const char* addr, int port)
{
    int fd;
    int iret;
    struct sockaddr_in svr_addr;
    int error;
    int flags = 1;

    socklen_t optlen = sizeof(error);

    if( NULL == addr || port <= 0 )
    {
        printf("addr is NULL\n");

        return -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if( -1 == fd )
    {
        printf("socket error, %s", strerror(errno) );

        return -1;
    }

    if( setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags)) < 0 )
    {
        printf("set_sock_option error: %s", strerror(errno) );

        close(fd);

        return -1;
    }

    iret = SetNoBlock(fd);

    if( -1 == iret )
    {
        printf("SetNoBlock error, %s\n", strerror(errno) );

        close(fd);

        return -1;
    }

    bzero(&svr_addr, sizeof(struct sockaddr_in));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(port);
    svr_addr.sin_addr.s_addr = inet_addr(addr);

    iret = connect(fd, (struct sockaddr*)&svr_addr, sizeof(struct sockaddr_in) );

    if( 0 == iret )
    {
        return fd;
    }
    else if ( iret < 0 && errno != EINPROGRESS )
    {
        printf("connect error, %s", strerror(errno) );

        close(fd);

        return -1;
    }

    while( 1 )
    {
        iret = Select(fd, 600, EPOLLIN | EPOLLOUT);

        if( iret > 0 )
        {
            if( getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &optlen) < 0 )
            {
                printf("getsockopt error, %s", strerror(errno) );

                close(fd);

                return -1;
            }

            if ( error )
            {
                if( EINPROGRESS == errno )
                {
                    printf("retry");

                    continue;
                }

                printf("getsockopt error, %s", strerror(errno) );

                close(fd);

                return -1;
            }

            return fd;
        }
        else if( EINPROGRESS != errno )
        {
            break;
        }
    }

    close(fd);

    return -1;
}

//从fd接收remain_len长度的数据，并存入prcv_buf中
int Receive(int fd, char* buf, int remain_len )
{
    unsigned long data_len = 0;
    int recv_len = 0;

    while( remain_len > 0 )
    {
        recv_len = recv(fd, buf + data_len, remain_len, 0);

        if( recv_len == 0 )
        {
            printf("fd = %d closed\n", fd);

            return -1;
        }
        else if( recv_len < 0)
        {
            //被中断，或者没有数据发送则继续
            if( errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR || errno == EINPROGRESS)
            {
                continue;
            }
            else
            {
                printf("recv error, %d %s\n", errno, strerror(errno) );

                return -1;
            }
        }

        data_len += recv_len;
        remain_len -= recv_len;
    }

    return 0;
}

int Send(int fd, const char* buf, int remain_len)
{
    int ilen = 0;
    int send_len = 0;

    //在10秒内发送remain_len长度的数据
    while( remain_len > 0 )
    {
        send_len = send(fd, buf + ilen, remain_len, 0);

        if( send_len <= 0 )
        {
            if( errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR || errno == EINPROGRESS)
            {
                continue;
            }
            else
            {
                printf("send error, %d %s\n", errno, strerror(errno) );

                return -1;
            }
        }

        ilen += send_len;
        remain_len -= send_len;
    }

    return 0;
}

int recvice_pkt(int fd, char* buf, int max_len)
{
    if( Receive(fd, buf, PACKAGE_CMD ) < 0 )
    {
        printf("recv head failed, %s\n", strerror(errno) );

        return  -1;
    }

    int len = *((__u32 *) (buf + PACKAGE_LEN));
    len = (int) ntohl(len);

    if( max_len < len )
    {
        printf("package is too long\n");

        return -1;
    }

    if( Receive(fd, buf + PACKAGE_CMD, len - PACKAGE_CMD) < 0 )
    {
        printf("recv body failed, %s\n", strerror(errno) );

        return -1;
    }

    return len;
}

void start(const char* addr, int port, const char* info)
{
    char buf[1024] = {0};

    struct pbpack* rpkt = CreatePack(buf, sizeof(buf) );

    //创建发送的包
    SetCmd(rpkt, 0x2900);
    AddAllData(rpkt, info, strlen(info) );
    PackToStr(rpkt);
    int send_len = GetPackLen(rpkt);

    int fd = Open(addr, port);

    if( fd <= 0 )
    {
        printf("Open %s:%d error, %s\n", addr, port, strerror(errno) );

        exit(0);
    }

    if( Send(fd, buf, send_len) )
    {
        printf("Send failed, %s\n", strerror(errno) );

        return;
    }

    char recv_buf[1024] = {0};

    int recv_len = recvice_pkt(fd, recv_buf, sizeof(recv_buf) );

    if( recv_len < 0 )
    {
        return;
    }

    struct pbpack* pkt = StrToPack(recv_buf, recv_len);

    const char* rcvbuf = GetDataPos(pkt);
    int size = GetDataSize(pkt);

    char show[1024] = {0};

    memcpy(show, rcvbuf, size);

    printf("RECV[%s]\n", show);

    return;
}

int main(int argc, char* argv[])
{
    //connsvr的IP connsvr的端口 发送的内容
    if( argc < 4 )
    {
        printf("./conntest ip port info\n");

        return -1;
    }

    start(argv[1], atoi(argv[2]), argv[3]);

    return 0;
}

