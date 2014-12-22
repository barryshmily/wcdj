/** Copyright(C)  1998-2014,Tencent Co. Ltd.
* File name:    event.cpp
* @author:      jiwubu
* @version:     V1.0   2014/09/14
* Description: 	事件处理框架
* Log:
*/

#include "event.h"

#define EPOLL_SIZE 2048

void file_event_init(event_base* base, int fd)
{
    file_event* fe = &base->fevents[fd];

    fe->mask = 0;
    fe->rproc = fe->wproc = NULL;
    fe->arg = NULL;

    return;
}

struct event_base *event_base_new(int size)
{
    event_base *base = (event_base*)malloc(sizeof(*base));

    if( NULL == base )
    {
        return NULL;
    }

    memset(base, 0x00, sizeof(event_base) );

    base->size = size;
    base->epfd = epoll_create(1024);

    if( base->epfd < 0 )
    {
        return NULL;
    }

    base->eevents = (struct epoll_event*)malloc( EPOLL_SIZE *
                    sizeof(struct epoll_event));

    if( NULL == base->eevents )
    {
        return NULL;
    }

    base->fevents = (struct file_event*)malloc(size*sizeof(struct file_event));

    if( NULL == base->fevents )
    {
        return NULL;
    }

    return base;
}

void event_base_del(event_base *base)
{
    free(base->eevents);
    free(base->fevents);
    free(base);

    return;
}

int event_add(event_base *base, int fd, int mask, event_proc *proc, void *arg)
{
    if( fd > base->size )
    {
        return AE_ERR;
    }

    struct file_event* fe = &base->fevents[fd];

    //已经注册，直接返回
    if( fe->mask & mask )
    {
        return AE_OK;
    }

    struct epoll_event ee;
    ee.events = 0;

    int op = fe->mask == AE_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    fe->mask |= mask;

    if (fe->mask & AE_READABLE)
    {
        ee.events |= EPOLLIN;
    }

    if (fe->mask & AE_WRITABLE)
    {
        ee.events |= EPOLLOUT;
    }

    if (mask & AE_READABLE)
    {
        fe->rproc = proc;
    }

    if (mask & AE_WRITABLE)
    {
        fe->wproc = proc;
    }

    fe->arg = arg;

    ee.data.fd = fd;

    if ( epoll_ctl(base->epfd, op, fd, &ee) == -1 )
    {
        return AE_ERR;
    }

    return AE_OK;
}

int event_del(event_base *base, int fd, int mask)
{
    struct file_event* fe = &base->fevents[fd];

    if ( fe->mask == AE_NONE)
    {
        return AE_OK;
    }

    //已经删除，直接返回
    if( !(fe->mask & mask) )
    {
        return AE_OK;
    }

    fe->mask = fe->mask & (~mask);

    struct epoll_event ee;
    ee.events = 0;

    if( fe->mask & AE_READABLE)
    {
        ee.events |= EPOLLIN;
    }

    if( fe->mask & AE_WRITABLE)
    {
        ee.events |= EPOLLOUT;
    }

    ee.data.fd = fd;

    int op = fe->mask == AE_NONE ? EPOLL_CTL_DEL : EPOLL_CTL_MOD;

    if ( epoll_ctl(base->epfd, op, fd, &ee) < 0 )
    {
        return AE_ERR;
    }

    return AE_OK;
}

void event_dispatch(event_base *base)
{
    int i = 0;
    int nfds;

    while( 1 )
    {
        nfds = epoll_wait(base->epfd, base->eevents, EPOLL_SIZE, -1);

        for( i = 0; i < nfds; i++)
        {
            struct epoll_event *e = base->eevents + i;

            int fd = e->data.fd;
            int mask = 0;
            file_event *fe = &base->fevents[fd];

            if( e->events & EPOLLIN)
            {
                mask |= AE_READABLE;
            }

            if( (e->events & EPOLLOUT) || (e->events & EPOLLERR)
                    || (e->events & EPOLLHUP) )
            {
                mask |= AE_WRITABLE;
            }

            if( fe->mask & mask & AE_READABLE)
            {
                fe->rproc(base, fd, fe->arg);
            }

            if( fe->mask & mask & AE_WRITABLE)
            {
                fe->wproc(base, fd, fe->arg);
            }
        }
    }

    return;
}

