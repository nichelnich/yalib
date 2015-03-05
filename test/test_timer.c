#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "yalib.h"

static int flag = 1;
static ya_timer_heap_t *heap;

void timer_cb(ya_timer_entry_t *entry)
{
	ya_time_val delay;
	void *usr_data = entry->get_usr_data(entry);
	int count = (int)usr_data;

	printf("-----call back:%d\n", count);

	if(count >= 10) {
		flag = 0;
		return ;
	}
	++ count;
	entry->set_usr_data(entry, (void *)count);

	delay.sec = 3;
	delay.msec = 0;
	entry->schedule(entry, heap, &delay);
}

#define TIMER_ID	1

int main(void)
{
	ya_timer_entry_t *entry;
	ya_time_val delay;

	heap = new("test_timer", sizeof(ya_timer_heap_t), ya_timer_heap_ctor, ya_timer_heap_dtor, -1);
	if(!heap) {
		printf("new heap error\n");
		return -1;
	}
	entry = new(NULL, sizeof(ya_timer_entry_t), ya_timer_entry_ctor, ya_timer_entry_dtor, TIMER_ID, (void *)1, timer_cb);
	if(!entry) {
		printf("new entry error\n");
		return -1;
	}

	delay.sec = 3;
	delay.msec = 0;
	entry->schedule(entry, heap, &delay);

	while (flag) {
		heap->poll(heap);
		usleep(200 * 1000);
	}

	unref_obj(entry);
	unref_obj(heap);

	return 0;
}
