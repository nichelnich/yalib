/*
 * brief
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include "ya/log.h"
#include "ya/util.h"


void ya_free(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}


int ya_pthread_mutex_lock(pthread_mutex_t *mutex, const char *file, int line)
{
	int ret;
#ifdef DEBUG_MUTEX
	ya_log(LOG_LEVEL_DEBUG, "LOCK_BEGIN--tid:%d, info:%s:%d", pthread_self(), file, line);
#endif
	ret = pthread_mutex_lock(mutex);
#ifdef DEBUG_MUTEX
	ya_log(LOG_LEVEL_DEBUG, "LOCK_END--tid:%d", pthread_self());
#endif
	return ret;
}

int ya_pthread_mutex_trylock(pthread_mutex_t *mutex, const char *file, int line)
{
	int ret;
#ifdef DEBUG_MUTEX
	ya_log(LOG_LEVEL_DEBUG, "TRYLOCK_BEGIN--tid:%d, info:%s:%d", pthread_self(), file, line);
#endif
	ret = pthread_mutex_trylock(mutex);
#ifdef DEBUG_MUTEX
	if(0 == ret) {
		ya_log(LOG_LEVEL_DEBUG, "TRYLOCK_END_SUCCEED--tid:%d", pthread_self());
	} else {
		ya_log(LOG_LEVEL_DEBUG, "TRYLOCK_END_FIALED--tid:%d", pthread_self());
	}
#endif
	return ret;
}

int ya_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	int ret;
	ret = pthread_mutex_unlock(mutex);
#ifdef DEBUG_MUTEX
	ya_log(LOG_LEVEL_DEBUG, "UNLOCK--tid:%d, info:%s", pthread_self());
#endif
	return ret;
}

ya_status_t ya_gettickcount(ya_time_val *tv)
{
	struct timespec tp;

	if (clock_gettime(CLOCK_MONOTONIC, &tp) != 0) {
		return YA_ERROR;
	}

	tv->sec = tp.tv_sec;
	tv->msec = tp.tv_nsec / 1000000;
    return YA_SUCCESS;
}

void ya_time_val_normalize(ya_time_val *t)
{
	if (t->msec >= 1000) {
		t->sec += (t->msec / 1000);
		t->msec = (t->msec % 1000);
	} else if (t->msec <= -1000) {
		do {
			t->sec--;
			t->msec += 1000;
		} while (t->msec <= -1000);
	}

	if (t->sec >= 1 && t->msec < 0) {
		t->sec--;
		t->msec += 1000;

	} else if (t->sec < 0 && t->msec > 0) {
		t->sec++;
		t->msec -= 1000;
	}
}

int ya_time_val_cmp(ya_time_val *t1, ya_time_val *t2)
{
	if(t1->msec + t1->sec * 1000 == t2->msec + t2->sec * 1000) {
		return 0;
	}else if(t1->msec + t1->sec * 1000 > t2->msec + t2->sec * 1000) {
		return 1;
	}else {
		return -1;
	}
}
