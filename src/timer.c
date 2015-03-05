/*
 * brief
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <stdarg.h>
#include <pthread.h>
#include "yalib.h"

#define MAX_ENTRIES_PER_HEAP 64

struct ya_timer_entry_private
{
    int id;
    ya_time_val _timer_value;
    void *user_data;
    ya_timer_heap_callback *cb;
    ya_timer_heap_t *heap;
};

struct ya_timer_heap_private
{
    unsigned max_entries;

    /** Lock object. */
    pthread_mutex_t *mutex;

    /** Autodelete lock. */
    ya_bool_t auto_delete_lock;

    /** heap */
    ya_timer_entry_t entrys;
};


#define HEAP_PRIVATE(obj) ((struct ya_timer_heap_private *)(((ya_timer_heap_t *)obj)->private))
#define ENTRY_PRIVATE(obj) ((struct ya_timer_entry_private *)(((ya_timer_entry_t *)obj)->private))


static void ya_timer_heap_set_lock (void *_this, pthread_mutex_t *lock, ya_bool_t auto_del)
{
	assert(_this);

	if(HEAP_PRIVATE(_this)->mutex && HEAP_PRIVATE(_this)->auto_delete_lock) {
		pthread_mutex_destroy(HEAP_PRIVATE(_this)->mutex);
	}
	HEAP_PRIVATE(_this)->mutex = lock;
	HEAP_PRIVATE(_this)->auto_delete_lock = auto_del;
}

static unsigned ya_timer_heap_poll (void *this)
{
	unsigned count = 0;
	ya_timer_entry_t *node;
	ya_time_val expires;

	assert(this);

	ya_gettickcount(&expires);

	if (HEAP_PRIVATE(this)->mutex) {
		YA_MUTEX_LOCK(HEAP_PRIVATE(this)->mutex);
	}

	while ((node = ya_list_get(&HEAP_PRIVATE(this)->entrys, 0))
			&& ya_time_val_cmp(&ENTRY_PRIVATE(node)->_timer_value, &expires) <= 0) {
		ya_list_erase(node);
		ENTRY_PRIVATE(node)->heap = NULL;

		if (HEAP_PRIVATE(this)->mutex) {
			YA_MUTEX_UNLOCK(HEAP_PRIVATE(this)->mutex);
		}

		if(ENTRY_PRIVATE(node)->cb) {
			ENTRY_PRIVATE(node)->cb(node);
		}

		if (HEAP_PRIVATE(this)->mutex) {
			YA_MUTEX_LOCK(HEAP_PRIVATE(this)->mutex);
		}
		++ count;
	}

	if (HEAP_PRIVATE(this)->mutex) {
		YA_MUTEX_UNLOCK(HEAP_PRIVATE(this)->mutex);
	}

	return count;
}

static int bigger(void *value, const void *node)
{
	ya_time_val *_value = (ya_time_val *)value;

	if(ya_time_val_cmp(&ENTRY_PRIVATE(node)->_timer_value, _value) > 0) {
		return 0;
	}
	return -1;
}

static ya_status_t ya_timer_entry_schedule (void *this, ya_timer_heap_t *heap, const ya_time_val *delay)
{
	ya_time_val expires;

	assert(this && heap && delay);

	ya_gettickcount(&expires);
	YA_TIME_VAL_ADD(expires, *delay);

	if(HEAP_PRIVATE(heap)->mutex) {
		YA_MUTEX_LOCK(HEAP_PRIVATE(heap)->mutex);
	}

	ENTRY_PRIVATE(this)->_timer_value.sec = expires.sec;
	ENTRY_PRIVATE(this)->_timer_value.msec = expires.msec;

	ENTRY_PRIVATE(this)->heap = heap;

	{
		ya_timer_entry_t *node = ya_list_search(&HEAP_PRIVATE(heap)->entrys, &expires, bigger);
		if(node) {
			ya_list_insert_before(node, this);
		}else {
			ya_list_insert_before(&HEAP_PRIVATE(heap)->entrys, this);
		}
	}

	if (HEAP_PRIVATE(heap)->mutex) {
		YA_MUTEX_UNLOCK(HEAP_PRIVATE(heap)->mutex);
	}
	return YA_SUCCESS;
}

static ya_status_t ya_timer_entry_cancel (void *this)
{
	assert(this);
	pthread_mutex_t *_mutex = HEAP_PRIVATE(ENTRY_PRIVATE(this)->heap)->mutex;

	if (_mutex) {
		YA_MUTEX_LOCK(_mutex);
	}

	ya_list_erase(this);
	ENTRY_PRIVATE(this)->heap = NULL;

	if (_mutex) {
		YA_MUTEX_UNLOCK(_mutex);
	}
	return YA_SUCCESS;
}

static int ya_timer_entry_get_id(void *this)
{
	assert(this);
	return ENTRY_PRIVATE(this)->id;
}

static void *ya_timer_entry_get_usr_data(void *this)
{
	assert(this);
	return ENTRY_PRIVATE(this)->user_data;
}

static ya_status_t ya_timer_entry_set_usr_data(void *this, void *usr_data)
{
	assert(this);
	ENTRY_PRIVATE(this)->user_data = usr_data;
	return YA_SUCCESS;
}


/*=================== heap ctor dtor start =====================*/
void *ya_timer_heap_ctor(void *obj, dtor_type *dtor, va_list *ap)
{
	ya_timer_heap_t *heap = (ya_timer_heap_t *)obj;
	unsigned _max_entries;

	assert(obj && dtor && ap);

	heap->private = calloc(1, sizeof(struct ya_timer_heap_private));
	if(!heap->private) {
		return NULL;
	}

	//public
	heap->dtor = dtor;
	heap->poll = ya_timer_heap_poll;
	heap->set_lock = ya_timer_heap_set_lock;

	//private
	_max_entries = va_arg(*ap, unsigned int);
	if(_max_entries < MAX_ENTRIES_PER_HEAP && _max_entries > 0) {
		HEAP_PRIVATE(obj)->max_entries = _max_entries;
	}else {
		HEAP_PRIVATE(obj)->max_entries = MAX_ENTRIES_PER_HEAP;
	}
	ya_list_init(&HEAP_PRIVATE(obj)->entrys);

	return obj;
}

void ya_timer_heap_dtor(void *obj)
{
	ya_timer_heap_t *heap = (ya_timer_heap_t *) obj;

	assert(obj);

	while(!ya_list_empty(&HEAP_PRIVATE(obj)->entrys)) {
		ya_timer_entry_t *node;
		node = ya_list_get(&HEAP_PRIVATE(obj)->entrys, 0);
		if(node) {
			node->cancel(node);
		}
	}

	if(HEAP_PRIVATE(obj)->mutex && HEAP_PRIVATE(obj)->auto_delete_lock) {
		pthread_mutex_destroy(HEAP_PRIVATE(obj)->mutex);
	}
	free(heap->private);
}
/*=================== heap ctor dtor end =====================*/

/*=================== entry ctor dtor start =====================*/
void *ya_timer_entry_ctor(void *obj, dtor_type *dtor, va_list *ap)
{
	ya_timer_entry_t *entry = (ya_timer_entry_t *)obj;

	assert(obj && dtor && ap);

	entry->private = calloc(1, sizeof(struct ya_timer_entry_private));
	if(!entry->private) {
		return NULL;
	}

	//public
	entry->dtor = dtor;
	entry->schedule = ya_timer_entry_schedule;
	entry->cancel = ya_timer_entry_cancel;
	entry->get_id = ya_timer_entry_get_id;
	entry->get_usr_data = ya_timer_entry_get_usr_data;
	entry->set_usr_data = ya_timer_entry_set_usr_data;

	//private
	ENTRY_PRIVATE(obj)->id = va_arg(*ap, int);
	ENTRY_PRIVATE(obj)->user_data = va_arg(*ap, void *);
	ENTRY_PRIVATE(obj)->cb = va_arg(*ap, ya_timer_heap_callback);

	return obj;
}

void ya_timer_entry_dtor(void *obj)
{
	ya_timer_entry_t *entry = (ya_timer_entry_t *)obj;

	assert(obj);
	// cancel entry
	if(!ya_list_original(entry)) {
		entry->cancel(entry);
	}
	free(entry->private);
}
/*=================== entry ctor dtor end =====================*/
