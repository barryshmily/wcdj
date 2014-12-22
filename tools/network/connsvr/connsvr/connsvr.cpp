/** Copyright(C)  1998-2014,Tencent Co. Ltd.
* File name:    tcpsvr.cpp
* @author:      jiwubu
* @version:     V1.0   2014/09/14
* Description: 	socket处理框架
* Log:
*/

#include "connsvr.h"
#include "process.h"

static void process_accept(event_base *base, int fd, void *privdata);

//扩大接收缓冲区
char* resize_rbuf(struct conn* c, int len)
{
    if( c->rtotal - c->rlen >= len )
    {
        return c->rbuf + c->rlen;
    }

    while( c->rtotal - c->rlen < len )
    {
        c->rtotal *= 2;
    }

    char* p = (char*)malloc(c->rtotal);

    memmove(p, c->rbuf, c->rlen);
    free(c->rbuf);
    c->rbuf = p;

    return c->rbuf + c->rlen;
}

//扩大发送缓冲区
char* resize_sbuf(struct conn* c, int len)
{
    if( c->stotal - c->slen >= len )
    {
        return c->sbuf + c->slen;
    }

    if( c->stotal - c->slen + c->spos > len )
    {
        memmove(c->sbuf, c->sbuf + c->spos, c->slen - c->spos);

        c->slen -= c->spos;
        c->spos = 0;

        return c->sbuf + c->slen;
    }

    while( (c->stotal - c->slen) < len )
    {
        c->stotal *= 2;
    }

    char* p = (char*)malloc(c->stotal);

    memmove(p, c->sbuf + c->spos, c->slen - c->spos);
    free(c->sbuf);

    c->sbuf = p;
    c->slen -= c->spos;
    c->spos = 0;

    return c->sbuf + c->slen;
}

//创建新的连接
static struct conn* create_conn(event_base* base, int fd, char* ip, int port)
{
    struct conn* c = (struct conn*)malloc(sizeof(struct conn) );

    if( NULL == c )
    {
        Err("malloc conn error: %s", strerror(errno) );

        exit(-1);
    }

    memset(c, 0x00, sizeof(struct conn) );

    c->rtotal = MAX_BUF_SIZE;
    c->stotal = MAX_BUF_SIZE;

    c->rbuf = (char*)malloc( sizeof(char) * c->rtotal);
    c->sbuf = (char*)malloc( sizeof(char) * c->stotal);

    c->base = base;
    c->fd = fd;
    c->port = port;
    memcpy(c->ip, ip, sizeof(c->ip) );

	//初始化时间事件的结构体
    c->tevent = (struct time_event*)malloc(sizeof(struct time_event));
    time_event_init(c->tevent, NULL, NULL);

    //初始化file_event
    file_event_init(base, c->fd);

    return c;
}

//销毁连接
void destroy_conn(struct conn* c)
{
    struct event_base* base = c->base;

    base->count--;

    Debug("[%d] %s:%d closed, count=%d", getpid(), c->ip, c->port, base->count);

    if( event_del(base, c->fd, AE_READABLE|AE_WRITABLE) != AE_OK )
    {
        Err("event_del failed: %s", strerror(errno) );

        exit(-1);
    }

    close(c->fd);

    unregister_timer(c->base, c->tevent);

    file_event_init(base, c->fd);

    free(c->rbuf);
    free(c->sbuf);
    free(c->tevent);
    free(c);

    return;
}

void conn_timeout(void* arg)
{
    struct conn* c = (struct conn*)arg;

    Warn("[%s:%d] timeout", c->ip, c->port);

    destroy_conn(c);

    return;
}

//数据接收事件处理函数,为提高性能此函数也协议完全耦合，先接收数据包头，再接收BODY
static void process_recv(event_base *base, int fd, void *privdata)
{
    struct conn *c = (conn*)privdata;
    int ret = 0;
    int remain_len;
    int len;

    //接收数据小于HEAD长度时，继续接收HEAD，否则接收BODY
    if ( c->rlen < PKG_HEAD_SIZE )
    {
        ret = anetRead(c->fd, c->rbuf + c->rlen, PKG_HEAD_SIZE - c->rlen);

        if( ret <= 0 )
        {
            goto err;
        }

        c->rlen += ret;

        if( c->rlen < PKG_HEAD_SIZE )
        {
            return;
        }

        //根据协议计算整个包的长度
        c->plen = *((__u32 *) (c->rbuf + PACKAGE_LEN));
        c->plen = (int) ntohl(c->plen);

        if( *c->rbuf != SOH || c->plen <= PACKAGE_BODY )
        {
            Err("[%s:%d] invalid package", c->ip, c->port);

            goto err;
        }

        if( c->plen >= MAX_PKT_SIZE )
        {
            Err("[%s:%d] package too long, len=%d", c->ip, c->port, c->plen);

            goto err;
        }

        resize_rbuf(c, c->plen - PKG_HEAD_SIZE);
    }

    //为防止其它fd饿死,如果包长小于4096,则接收整个包，否则一次只接收4096个字节
    remain_len = c->plen - c->rlen;
    len = remain_len > 4096 ? 4096 : remain_len;

    ret = anetRead(c->fd, c->rbuf + c->rlen, len);

    if( ret <= 0 )
    {
        goto err;
    }

    c->rlen += ret;

    //为达到一个包的长度时，继续接收
    if( c->rlen < c->plen )
    {
        return;
    }

    //调用处理函数进行处理
    process_packet(c, c->rbuf, c->rlen);

    //重新注册连接超时定时器
    register_timer(base, c->tevent, MAX_TIMEOUT);

    //处理完一个数据包后，缓冲区又可以重复使用了
    c->rlen = 0;
    c->plen = 0;

    return;

err:
    destroy_conn(c);

    return;
}

static void process_send(event_base *base, int fd, void *privdata)
{
    struct conn *c = (conn*)privdata;

    int nbytes = anetWrite(c->fd, c->sbuf, c->slen);

    if ( nbytes >= 0 )
    {
        c->spos += nbytes;
    }
    else
    {
        Err("[%s:%d] write error: %s", c->ip, c->port, strerror(errno));

        destroy_conn(c);

        return;
    }

    //数据全部发送完成
    if( c->spos == c->slen )
    {
        if( event_del(base, c->fd, AE_WRITABLE) != AE_OK)
        {
            Err("event_del failed: %s", strerror(errno) );

            exit(-1);
        }
    }

    c->slen = 0;
    c->spos = 0;

    return;
}

//客户端连接事件回调函数
//为了防止同时并发大量的新连接，可以将系统参数/proc/sys/net/core/somaxconn设置大点
static void process_accept(event_base *base, int fd, void *privdata)
{
    int cport, cfd;
    char cip[128];
    char error_[1024];

    //接收新的客户端
    cfd = anetTcpAccept(error_, fd, cip, &cport);

    if( cfd == AE_ERR )
    {
        if( errno != EAGAIN && errno != EWOULDBLOCK )
        {
            Err("[%d] anetTcpAccept failed: %s", getpid(), strerror(errno) );

            exit(-1);
        }

        return;
    }

    base->count++;

    //设置为非阻塞模式
    if( anetNonBlock(error_, cfd) == AE_ERR )
    {
        Err("anetNonBlock failed: %s", error_);

        exit(-1);
    }

    Debug("[%d] %s:%d connected, count=%d", getpid(), cip, cport, base->count);

    struct conn *c = create_conn(base, cfd, cip, cport);

    //注册连接超时定时器
    time_event_init(c->tevent, conn_timeout, c);
    register_timer(base, c->tevent, MAX_TIMEOUT);

    //注册新连接读IO事件到事件框架
    if( event_add(base, cfd, AE_READABLE, process_recv, c) == AE_ERR)
    {
        Err("[%d] %s:%d event_add failed, %s", getpid(), cip, cport, strerror(errno) );

        exit(-1);
    }

    return;
}

//向conn增加需要发送的包,并注册写的IO事件
void append_pkt(struct conn* c, int len, char* buf)
{
    if( buf )
    {
        memcpy(c->sbuf + c->slen, buf, len);
    }

    c->slen += len;

    if( event_add(c->base, c->fd, AE_WRITABLE, process_send, c) == AE_ERR )
    {
        Err("[%d] %s:%d event_add failed, %s", c->ip, c->port, strerror(errno) );

        exit(-1);
    }

    return;
}

//初始化事件驱动，并将listenfd的读IO事件添加到事件驱动中，设置回调函数为process_accept
struct event_base* event_base_init(int fd)
{
    struct event_base* base = event_base_new(MAX_CLIENT_COUNT);

    if( NULL == base )
    {
        Err("Can't create file_event loop: %s", strerror(errno) );

        return NULL;
    }

    base->fd = fd;

    if ( event_add(base, fd, AE_READABLE, process_accept, NULL) == AE_ERR )
    {
        Err("Can't create file file_event: %s", strerror(errno) );

        return NULL;
    }

    return base;
}

//登陆监控服务器
int create_server(const char* ip, int port)
{
    char error[1024] = {0};

    int fd = anetTcpServer(error, port, ip);

    if( fd < 0 )
    {
        Err("Can't create socket: %s", error );

        return -1;
    }

    Info("start to listen %s:%d", ip, port);

    return fd;
}

