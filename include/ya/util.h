/*
 * brief
 */

#ifndef __YA_UTIL_H__
#define __YA_UTIL_H__

void ya_debug_assert(int expression);

#define MY_MUTEX_LOCK(mutex)  my_pthread_mutex_lock(mutex, __FILE__ __LINE__)
#define MY_MUTEX_TRYLOCK(mutex)  my_pthread_mutex_trylock(mutex, __FILE__ __LINE__)
#define MY_MUTEX_UNLOCK(mutex) my_pthread_mutex_umloc(mutex)

#endif
