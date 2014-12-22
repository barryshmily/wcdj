/** Copyright(C)  1998-2014,Tencent Co. Ltd.
* File name:    tcpsvr.cpp
* @author:      jiwubu
* @version:     V1.0   2014/09/14
* Description: 	socket处理框架
* Log:
*/

#include "connsvr.h"

static void process_recv(event_base *base, int fd, void *privdata);
static void process_send(event_base *base, int fd, void *privdata);
static void process_accept(event_base *base, int fd, void *privdata);

void destroy_remote(conn *r)
{
    if( event_del(r->base, r->fd, AE_READABLE|AE_WRITABLE) != AE_OK )
    {
        Err("event_del failed: %s", strerror(errno) );
    }

    close(r->fd);
    free(r);

    return;
}

void destroy_conn(conn *c)
{
    struct event_base* base = c->base;

    base->count--;

    Debug("%s:%d=>%s:%d closed, count=%d", c->ip, c->port,
          c->remote->ip, c->remote->port, base->count);

    if( event_del(base, c->fd, AE_READABLE|AE_WRITABLE) != AE_OK )
    {
        Err("event_del failed: %s", strerror(errno) );
    }

    close(c->fd);

    destroy_remote(c->remote);

    free(c);

    return;
}

void remote_error(conn *c)
{
    c->remote->OnError(c->remote);

    return;
}

struct conn *create_remote(struct event_base* base, conn *c)
{
    char error_[1024] = {0};

    struct conn *r = (struct conn*)malloc(sizeof(struct conn));

    if( NULL == r )
    {
        return NULL;
    }

    r->port = G_ProxyPort;
    memcpy(r->ip, G_ProxyIP, sizeof(r->ip) );

    int fd = anetTcpNonBlockConnect(error_, r->ip, r->port);

    if( fd == ANET_ERR )
    {
        free(r);

        return NULL;
    }

    anetNonBlock(NULL, fd);
    anetTcpNoDelay(NULL, fd);

    r->base = base;
    r->fd = fd;

    r->total = MAX_BUF_SIZE;
    r->wpos = 0;
    r->rpos = 0;

    r->remote = c;
    r->OnError = remote_error;

    if( event_add(base, fd, AE_READABLE, process_recv, r) == AE_ERR )
    {
        Err("Can't create file file_event: %s", strerror(errno) );

        close(fd);
        free(r);

        return NULL;
    }

    return r;
}

conn *create_conn(struct event_base* base, int fd, char* ip, int port)
{
    struct conn *c = (struct conn*)malloc(sizeof(conn));

    if( c == NULL )
    {
        return NULL;
    }

    anetNonBlock(NULL, fd);
    anetTcpNoDelay(NULL, fd);

    c->base = base;
    c->fd = fd;

    c->total = MAX_BUF_SIZE;
    c->rpos = 0;
    c->wpos = 0;

    c->port = port;
    memcpy(c->ip, ip, sizeof(c->ip) );

    c->OnError = destroy_conn;
    c->remote = create_remote(base, c);

    if( NULL == c->remote )
    {
        close(fd);
        free(c);

        return NULL;
    }

    Debug("new client %s:%d=>%s:%d", c->ip, c->port,
          c->remote->ip, c->remote->port);

    return c;
}

//数据接收事件处理函数
static void process_recv(event_base *base, int fd, void *privdata)
{
    struct conn *c = (conn*)privdata;
    struct conn *r = c->remote;
    int ret = 0;

    //缓冲已满，等待发送
    if( r->total - r->wpos <= 0 )
    {
        event_del(c->base, c->fd, AE_READABLE);

        return;
    }

    //将接收的数据放到对端的BUF
    ret = anetRead(c->fd, r->buf + r->wpos, r->total- r->wpos);

    if( ret <= 0 )
    {
        goto err;
    }

    r->wpos += ret;

    if( event_add(r->base, r->fd, AE_WRITABLE, process_send, r) == AE_ERR )
    {
        Err("[%d] %s:%d event_add failed, %s", r->ip, r->port, strerror(errno) );

        exit(-1);
    }

    return;

err:
    c->OnError(c);

    return;
}

static void process_send(event_base *base, int fd, void *privdata)
{
    struct conn *c = (conn*)privdata;
    struct conn *r = c->remote;

    int nbytes = anetWrite(c->fd, c->buf + c->rpos, c->wpos - c->rpos);

    if ( nbytes >= 0 )
    {
        c->rpos += nbytes;
    }
    else
    {
        Err("[%s:%d] write error: %s", c->ip, c->port, strerror(errno));

        c->OnError(c);

        return;
    }

    //数据全部发送完成
    if( c->wpos == c->rpos )
    {
        if( event_del(base, c->fd, AE_WRITABLE) != AE_OK)
        {
            Err("event_del failed: %s", strerror(errno) );

            exit(-1);
        }

        c->wpos = 0;
        c->rpos = 0;
    }

    if( c->total - c->wpos > 0 )
    {
        if( event_add(base, r->fd, AE_READABLE, process_recv, r) == AE_ERR )
        {
            Err("%s:%d event_add failed, %s", r->ip, r->port, strerror(errno) );

            c->OnError(c);
        }
    }

    return;
}

//客户端连接事件处理函数
static void process_accept(event_base *base, int fd, void *privdata)
{
    int cport, cfd;
    char cip[128];
    char error_[1024];

    cfd = anetTcpAccept(error_, fd, cip, &cport);

    if( cfd == AE_ERR )
    {
        //不是被其它进程抢占了，则报错，退出进程重启
        if( errno != EAGAIN && errno != EWOULDBLOCK )
        {
            Err("[%d] anetTcpAccept failed: %s", getpid(), strerror(errno) );

            exit(-1);
        }

        return;
    }

    base->count++;

    Debug("%s:%d connected, count=%d", cip, cport, base->count);

    struct conn* c = create_conn(base, cfd, cip, cport);

    if( c == NULL )
    {
        return;
    }

    if( event_add(base, cfd, AE_READABLE, process_recv, c) == AE_ERR )
    {
        Err("%s:%d event_add failed, %s", cip, cport, strerror(errno) );

        c->OnError(c);
    }

    return;
}

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

struct event_base* event_base_init(int fd)
{
    struct event_base* base = event_base_new(MAX_CLIENT_COUNT);

    if( NULL == base )
    {
        Err("Can't create file_event loop: %s", strerror(errno) );

        return NULL;
    }

    if ( event_add(base, fd, AE_READABLE, process_accept, NULL) == AE_ERR )
    {
        Err("Can't create file file_event: %s", strerror(errno) );

        return NULL;
    }

    return base;
}

