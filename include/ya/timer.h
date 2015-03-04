/*
 * brief
 */

#ifndef __TIMER_H__
#define __TIMER_H__

typedef struct _ya_timer_heap{
	struct BASE_CLASS_OBJECT _;
	CLASS_ATTR_DATE;

	void (*set_lock) (void *this, pthread_mutex_t *lock, ya_bool_t auto_del);
	unsigned (*poll) (void *this, ya_time_val *next_delay);
}ya_timer_heap_t;

typedef struct _ya_timer_entry{
	struct BASE_CLASS_OBJECT _;
	CLASS_ATTR_DATE;

	ya_status_t (*schedule) (void *_this, ya_timer_heap_t *heap, const ya_time_val *delay);
	int (*cancel) (void *_this);
}ya_timer_entry_t;

typedef void ya_timer_heap_callback(ya_timer_heap_t *timer_heap,
		ya_timer_entry_t *entry);

#endif
