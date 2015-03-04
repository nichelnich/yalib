/*
 * brief
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "ya/util.h"
#include "ya/timer.h"

#define MAX_ENTRIES_PER_HEAP 64

struct ya_timer_entry_private
{

    int id;
    int _timer_id;
    struct timeval _timer_value;
    void *user_data;
    ya_timer_heap_callback *cb;
};

struct ya_timer_heap_private
{
    unsigned max_entries;

    /** Lock object. */
    pthread_mutex_t *mutex;

    /** Autodelete lock. */
    ya_bool_t auto_delete_lock;

    /** heap */
    ya_timer_entry_t *heap;

    unsigned char timer_ids[MAX_ENTRIES_PER_HEAP % 8 == 0 ? (MAX_ENTRIES_PER_HEAP/8) : (MAX_ENTRIES_PER_HEAP/8 + 1)];
};
