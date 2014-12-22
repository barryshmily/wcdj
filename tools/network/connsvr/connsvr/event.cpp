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

    base->tv_cache = (struct timeval*)malloc(sizeof(struct timeval));
    base->heap = (struct min_heap*)malloc(sizeof(struct min_heap));

    if( NULL == base->tv_cache || NULL == base->heap )
    {
        return NULL;
    }

    base->tv_cache->tv_sec = base->tv_cache->tv_usec = 0;

    min_heap_ctor(base->heap);

    gettimeofday(base->tv_cache, NULL);

    return base;
}

void event_base_del(event_base *base)
{
    free(base->eevents);
    free(base->fevents);
    free(base);

    return;
}

void event_stop(event_base *base)
{
    base->stop = 1;
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

//注册计时器
void register_timer(struct event_base* base, struct time_event* ev, int usec)
{
    ev->interval.tv_sec  = usec / 1000;
    ev->interval.tv_usec = usec % 1000;

    evutil_timeradd(base->tv_cache, &ev->interval, &ev->timeout);

    min_heap_erase(base->heap, ev);
    min_heap_push(base->heap, ev);

    return;
}

//删除已经注册的计时器
void unregister_timer(struct event_base* base, struct time_event* ev)
{
    min_heap_erase(base->heap, ev);

    return;
}

//获取下一个超时事件的时间
int timeout_next(struct event_base *base, struct timeval *tv)
{
    struct timeval* now = base->tv_cache;
    struct time_event *ev;

    ev = min_heap_top(base->heap);

    if ( NULL == ev )
    {
        return -1;
    }

    if( 0 == base->tv_cache->tv_sec )
    {
        gettimeofday(base->tv_cache, NULL);
    }

    //超时时间已经超过当前时间,不需要等待
    if (evutil_timercmp(&ev->timeout, now, <=))
    {
        tv->tv_sec = tv->tv_usec = 0;

        return 0;
    }

    evutil_timersub(&ev->timeout, now, tv);

    //Info("timeout_next: in %d seconds", (int)tv->tv_sec);

    return 0;
}

//处理超时事件
int process_timeout(struct event_base* base)
{
    struct timeval* now = base->tv_cache;
    struct time_event *ev;

    while( ( ev = min_heap_top(base->heap)) != NULL )
    {
        if( evutil_timercmp(&ev->timeout, now, >) )
        {
            break;
        }

        //删除定时器
        min_heap_pop(base->heap);

        //调用对应超时事件的处理函数
        if( ev->proc )
        {
            ev->proc(ev->arg);
        }
    }

    return 0;
}

void event_dispatch(event_base *base)
{
    int i = 0;
    int timeout = -1;
    struct timeval tv;
    int nfds;
    base->stop = 0;

    while( !base->stop )
    {
        if( timeout_next(base, &tv) < 0 )
        {
            timeout = -1;
        }
        else
        {
            timeout = (tv.tv_sec * 1000) + ((tv.tv_usec + 999) / 1000);
        }

        nfds = epoll_wait(base->epfd, base->eevents, EPOLL_SIZE, timeout);

        gettimeofday(base->tv_cache, NULL);

        if( nfds < 0 )
        {
            continue;
        }

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

        process_timeout(base);
    }

    return;
}

