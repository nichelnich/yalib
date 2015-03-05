/*
 * brief
 */

#ifndef __YA_UTIL_H__
#define __YA_UTIL_H__

void ya_debug_assert(int expression);

#define YA_MUTEX_LOCK(mutex)  ya_pthread_mutex_lock(mutex, __FILE__, __LINE__)
#define YA_MUTEX_TRYLOCK(mutex)  ya_pthread_mutex_trylock(mutex, __FILE__, __LINE__)
#define YA_MUTEX_UNLOCK(mutex) ya_pthread_mutex_unlock(mutex)


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

#define YA_TIME_VAL_ADD(t1, t2)	    do {			    \
					(t1).sec += (t2).sec;	    \
					(t1).msec += (t2).msec;	    \
					ya_time_val_normalize(&(t1)); \
				    } while (0)

/** Boolean. */
typedef int		ya_bool_t;

ya_status_t ya_gettickcount(ya_time_val *tv);
int ya_pthread_mutex_lock(pthread_mutex_t *mutex, const char *file, int line);
int ya_pthread_mutex_trylock(pthread_mutex_t *mutex, const char *file, int line);
int ya_pthread_mutex_unlock(pthread_mutex_t *mutex);
void ya_time_val_normalize(ya_time_val *t);
int ya_time_val_cmp(ya_time_val *t1, ya_time_val *t2);

#endif
