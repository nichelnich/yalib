/*
 * brief
 */

#ifndef __TIMER_H__
#define __TIMER_H__

typedef struct {

}ya_timer_heap_t;

typedef struct {

}ya_timer_entry_t;

typedef void ya_timer_heap_callback(ya_timer_heap_t *timer_heap,
		ya_timer_entry_t *entry);

#endif
