#ifndef MINHEAP_H_H
#define MINHEAP_H_H

#include <time.h>
#include <stdlib.h>

typedef void timeout_proc(void *arg);

typedef struct time_event
{
    int index;
    struct timeval timeout;

    struct timeval interval;

    timeout_proc* proc;
    void* arg;
} timeout_event;

typedef struct min_heap
{
    struct time_event** p;
    unsigned n, a;
} min_heap_t;

/** Return true iff the tvp is related to uvp according to the relational
 * operator cmp.  Recognized values for cmp are ==, <=, <, >=, and >. */
#define	evutil_timercmp(tvp, uvp, cmp)					\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?				\
	 ((tvp)->tv_usec cmp (uvp)->tv_usec) :				\
	 ((tvp)->tv_sec cmp (uvp)->tv_sec))

#define evutil_timeradd(tvp, uvp, vvp)					\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;       \
		if ((vvp)->tv_usec >= 1000000) {			\
			(vvp)->tv_sec++;				\
			(vvp)->tv_usec -= 1000000;			\
		}							\
	} while (0)

#define	evutil_timersub(tvp, uvp, vvp)					\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)

void min_heap_ctor(min_heap_t* s);
void min_heap_dtor(min_heap_t* s);
void time_event_init(struct time_event* e, timeout_proc* proc, void* arg);
int	 min_heap_elt_is_top(const struct time_event *e);
int	 min_heap_elem_greater(struct time_event *a, struct time_event *b);
int	 min_heap_empty(min_heap_t* s);
unsigned min_heap_size(min_heap_t* s);
struct time_event*  min_heap_top(min_heap_t* s);
int	min_heap_reserve(min_heap_t* s, unsigned n);
int	min_heap_push(min_heap_t* s, struct time_event* e);
struct time_event*  min_heap_pop(min_heap_t* s);
int	min_heap_erase(min_heap_t* s, struct time_event* e);
void min_heap_shift_up_(min_heap_t* s, unsigned hole_index, struct time_event* e);
void min_heap_shift_down_(min_heap_t* s, unsigned hole_index, struct time_event* e);

#endif

