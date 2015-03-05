/*
 * brief
 */

#ifndef __TIMER_H__
#define __TIMER_H__

typedef struct _ya_timer_heap{
	struct BASE_CLASS_OBJECT _;
	CLASS_ATTR_DATE;

	void (*set_lock) (void *_this, pthread_mutex_t *lock, ya_bool_t auto_del);
	/**
	 * Poll the timer heap, check for expired timers and call the callback for
	 * each of the expired timers.
	 *
	 * @return           The number of timers expired.
	 */
	unsigned (*poll) (void *_this);
}ya_timer_heap_t;

typedef struct _ya_timer_entry{
	struct BASE_CLASS_OBJECT _;
	CLASS_ATTR_DATE;

	ya_status_t (*schedule) (void *_this, ya_timer_heap_t *heap, const ya_time_val *delay);
	ya_status_t (*cancel) (void *_this);
	int (*get_id) (void *_this);
	void *(*get_usr_data) (void *_this);
	ya_status_t (*set_usr_data) (void *_this, void *usr_data);
}ya_timer_entry_t;

typedef void ya_timer_heap_callback(ya_timer_entry_t *entry);


void *ya_timer_heap_ctor(void *obj, dtor_type *dtor, va_list *ap);
void ya_timer_heap_dtor(void *obj);
void *ya_timer_entry_ctor(void *obj, dtor_type *dtor, va_list *ap);
void ya_timer_entry_dtor(void *obj);

#endif
