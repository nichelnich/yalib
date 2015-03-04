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

    unsigned char timer_ids[MAX_ENTRIES_PER_HEAP % 8 == 0 ? (MAX_ENTRIES_PER_HEAP/8) : (MAX_ENTRIES_PER_HEAP/8 + 1)];
};


#define HEAP_PRIVATE(obj) ((struct ya_timer_heap_private *)(((ya_timer_heap_t *)obj)->private))
#define ENTRY_PRIVATE(obj) ((struct ya_timer_entry_private *)(((ya_timer_entry_t *)obj)->private))


static void ya_timer_heap_set_lock (void *_this, pthread_mutex_t *lock, ya_bool_t auto_del)
{
	assert(_this);
	HEAP_PRIVATE(_this)->mutex = lock;
	HEAP_PRIVATE(_this)->auto_delete_lock = auto_del;
}

static unsigned ya_timer_heap_poll (void *_this, ya_time_val *next_delay)
{
	ya_timer_heap_t *this = (ya_timer_heap_t *)_this;

	assert(_this && next_delay);
}

static ya_status_t ya_timer_entry_schedule (void *_this, ya_timer_heap_t *heap, const ya_time_val *delay)
{
	ya_timer_entry_t *this = (ya_timer_entry_t *)_this;

	assert(_this && heap && delay);
}

static int ya_timer_entry_cancel (void *_this)
{
	ya_timer_entry_t *this = (ya_timer_entry_t *)_this;

	assert(_this);
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
	if(_max_entries < MAX_ENTRIES_PER_HEAP) {
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
