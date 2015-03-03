/*
 * brief
 */

#ifndef __YA_LIST_H__
#define __YA_LIST_H__

typedef struct {
	void *prev;
	void *next;
}ya_list_node_t;

void ya_list_init(void *node);

#endif
