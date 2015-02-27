/*
 * brief
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define YA_LOG printf

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
	YA_LOG(“LOCK_BEGIN--tid:%d, info:%s”, pthread_self(), info);
#endif
	ret = pthread_mutex_lock(mutex);
#ifdef DEBUG_MUTEX
	YA_LOG(“LOCK_END--tid:%d”, phread_self());
#endif
	return ret;
}

int ya_pthread_mutex_trylock(pthread_mutex_t *mutex, char *info)
{
	int ret;
#ifdef DEBUG_MUTEX
	YA_LOG(“TRYLOCK_BEGIN--tid:%d, info:%s”, pthread_self(), info);
#endif
	ret = pthread_mutex_trylock(mutex);
#ifdef DEBUG_MUTEX
	if(0 == ret) {
		YA_LOG(“TRYLOCK_END_SUCCEED--tid:%d”, phread_self());
	} else {
		YA_LOG(“TRYLOCK_END_FIALED--tid:%d”, phread_self());
	}
#endif
	return ret;
}

int ya_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	int ret;
	ret = pthread_mutex_unlock(mutex);
#ifdef DEBUG_MUTEX
	YA_LOG(“UNLOCK--tid:%d, info:%s”, pthread_self());
#endif
	return ret;
}



