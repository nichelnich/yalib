/*
 * brief
 */

#ifndef __YA_UTIL_H__
#define __YA_UTIL_H__

void ya_debug_assert(int expression);

#define MY_MUTEX_LOCK(mutex)  my_pthread_mutex_lock(mutex, __FILE__ __LINE__)
#define MY_MUTEX_TRYLOCK(mutex)  my_pthread_mutex_trylock(mutex, __FILE__ __LINE__)
#define MY_MUTEX_UNLOCK(mutex) my_pthread_mutex_umloc(mutex)


typedef int ya_status_t;
enum {
	YA_SUCCESS = 0,
	YA_ERROR,
};

typedef struct ya_time_val
{
    /** seconds */
    long    sec;
    /** milliseconds */
    long    msec;
} ya_time_val;

/** Boolean. */
typedef int		ya_bool_t;

ya_status_t ya_gettickcount(ya_time_val *tv);

#endif
