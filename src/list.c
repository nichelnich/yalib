#include <stdio.h>
#include <stdlib.h>
#include "ya/list.h"

void ya_list_init(void *node)
{
	ya_list_t *node_i = (ya_list_t *)node;
	node_i->prev = node;
	node_i->next = node;
}

int ya_list_empty(const void *node)
{
    return ((ya_list_t*)node)->next == node;
}

int ya_list_original(const void *node)
{
	return ((ya_list_t*)node)->next == node && ((ya_list_t*)node)->prev == node;
}

static void ya_link_node(void *prev, void *next)
{
    ((ya_list_t*)prev)->next = next;
    ((ya_list_t*)next)->prev = prev;
}

	int ya_list_size(const void *list)
{
    const ya_list_t *node = (const ya_list_t*) ((const ya_list_t*)list)->next;
    int count = 0;

    while (node && node != list) {
	++count;
	node = (ya_list_t*)node->next;
    }

    return count;
}

void ya_list_erase(void *node)
{
	ya_link_node( ((ya_list_t*)node)->prev, ((ya_list_t*)node)->next);

	ya_list_init(node);
}

void * ya_list_search(void *list, void *value,
	       		int (*comp)(void *value, const void *node))
{
	ya_list_t *p = (ya_list_t *) ((ya_list_t*)list)->next;
    while (p != list && (*comp)(value, p) != 0)
	p = (ya_list_t *) p->next;

    return p==list ? NULL : p;
}

void *ya_list_get(const void *list, int pos)
{
	ya_list_t *node = (ya_list_t*) ((const ya_list_t*)list)->next;
	int count = 0;

	while (node && node != list) {
		if(count == pos) {
			return node;
		}
		++count;
		node = (ya_list_t*)node->next;
	}

	return NULL;
}

void ya_list_insert_after(void *pos, void *node)
{
    ((ya_list_t*)node)->prev = pos;
    ((ya_list_t*)node)->next = ((ya_list_t*)pos)->next;
    //barrier();
    ((ya_list_t*) ((ya_list_t*)pos)->next)->prev = node;
    ((ya_list_t*)pos)->next = node;
}

void ya_list_insert_before(void *pos, void *node)
{
    ((ya_list_t*)node)->prev = ((ya_list_t*)pos)->prev;
    ((ya_list_t*)node)->next = pos;
    //barrier();
    ((ya_list_t*) ((ya_list_t*)pos)->prev)->next = node;
    ((ya_list_t*)pos)->prev = node;
}

int ya_list_add(void *list, void *node, int pos)
{
	ya_list_t *cur_node = (ya_list_t*) ((ya_list_t*)list)->next;
	int count = 0;

	if (list == NULL || node == NULL)
		return -1;

	if (pos == -1 || pos >= ya_list_size(list))
	{                           /* insert at the end  */
		ya_list_insert_after(list, node);
		return 0;
	}

	while (cur_node != list && count < pos) {
		++count;
		cur_node = (ya_list_t*)cur_node->next;
	}

	ya_list_insert_before(cur_node, node);

	return 0;
}

