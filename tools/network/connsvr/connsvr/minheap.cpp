/** Copyright(C)  1998-2014,Tencent Co. Ltd.
* File name:    minheap.cpp
* @author:      jiwubu
* @version:     V1.0   2014/09/14
* Description: 	堆操作，根据redis开源版本修改
* Log:
*/

#include "minheap.h"

int min_heap_elem_greater(struct time_event *a, struct time_event *b)
{
    return evutil_timercmp(&a->timeout, &b->timeout, >);
}

void min_heap_ctor(min_heap_t* s)
{
    s->p = 0;
    s->n = 0;
    s->a = 0;
}
void min_heap_dtor(min_heap_t* s)
{
    if (s->p)
    {
        free(s->p);
    }
}

void time_event_init(struct time_event* e, timeout_proc* proc, void* arg)
{
    e->index = -1;
    e->proc = proc;
    e->arg  = arg;
}

int min_heap_empty(min_heap_t* s)
{
    return 0u == s->n;
}

unsigned min_heap_size(min_heap_t* s)
{
    return s->n;
}

struct time_event* min_heap_top(min_heap_t* s)
{
    return s->n ? *s->p : 0;
}

int min_heap_push(min_heap_t* s, struct time_event* e)
{
    if (min_heap_reserve(s, s->n + 1))
    {
        return -1;
    }

    min_heap_shift_up_(s, s->n++, e);

    return 0;
}

struct time_event* min_heap_pop(min_heap_t* s)
{
    if (s->n)
    {
        struct time_event* e = *s->p;
        min_heap_shift_down_(s, 0u, s->p[--s->n]);
        e->index = -1;

        return e;
    }

    return 0;
}

int min_heap_elt_is_top(const struct time_event *e)
{
    return e->index == 0;
}

int min_heap_erase(min_heap_t* s, struct time_event* e)
{
    if (-1 != e->index)
    {
        struct time_event *last = s->p[--s->n];
        unsigned parent = (e->index - 1) / 2;
        /* we replace e with the last element in the heap.  We might need to
           shift it upward if it is less than its parent, or downward if it is
           greater than one or both its children. Since the children are known
           to be less than the parent, it can't need to shift both up and
           down. */
        if (e->index > 0 && min_heap_elem_greater(s->p[parent], last))
        {
            min_heap_shift_up_(s, e->index, last);
        }
        else
        {
            min_heap_shift_down_(s, e->index, last);
        }

        e->index = -1;

        return 0;
    }

    return -1;
}

int min_heap_reserve(min_heap_t* s, unsigned n)
{
    if (s->a < n)
    {
        struct time_event** p;
        unsigned a = s->a ? s->a * 2 : 8;

        if (a < n)
        {
            a = n;
        }

        if (!(p = (struct time_event**)realloc(s->p, a * sizeof *p)))
        {
            return -1;
        }

        s->p = p;
        s->a = a;
    }

    return 0;
}

void min_heap_shift_up_(min_heap_t* s, unsigned hole_index, struct time_event* e)
{
    unsigned parent = (hole_index - 1) / 2;
    while (hole_index && min_heap_elem_greater(s->p[parent], e))
    {
        (s->p[hole_index] = s->p[parent])->index = hole_index;
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    }

    (s->p[hole_index] = e)->index = hole_index;

    return;
}

void min_heap_shift_down_(min_heap_t* s, unsigned hole_index, struct time_event* e)
{
    unsigned min_child = 2 * (hole_index + 1);

    while (min_child <= s->n)
    {
        min_child -= min_child == s->n || min_heap_elem_greater(s->p[min_child], s->p[min_child - 1]);
        if (!(min_heap_elem_greater(e, s->p[min_child])))
        {
            break;
        }

        (s->p[hole_index] = s->p[min_child])->index = hole_index;
        hole_index = min_child;
        min_child = 2 * (hole_index + 1);
    }

    (s->p[hole_index] = e)->index = hole_index;

    return;
}

