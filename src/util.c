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

/**
 * @brief just use for debug, where associated with process should call assert(2),because crash more safe.
 */
void ya_debug_assert(int expression) /*boolen*/
{
#ifdef DEBUG_ASSERT
	assert(expression);
#endif
}


void ya_free(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}


int ya_pthread_mutex_lock(pthread_mutex_t *mutex, char *info)
{
	int ret;
#ifdef DEBUG_MUTEX
	ya_log(LOG_LEVEL_DEBUG, "LOCK_BEGIN--tid:%d, info:%s", pthread_self(), info);
#endif
	ret = pthread_mutex_lock(mutex);
#ifdef DEBUG_MUTEX
	ya_log(LOG_LEVEL_DEBUG, "LOCK_END--tid:%d", pthread_self());
#endif
	return ret;
}

int ya_pthread_mutex_trylock(pthread_mutex_t *mutex, char *info)
{
	int ret;
#ifdef DEBUG_MUTEX
	ya_log(LOG_LEVEL_DEBUG, "TRYLOCK_BEGIN--tid:%d, info:%s", pthread_self(), info);
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

