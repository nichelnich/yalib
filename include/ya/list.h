/*
 * brief
 */

#ifndef __YA_LIST_H__
#define __YA_LIST_H__

typedef struct {
	void *prev;
	void *next;
}ya_list_t;

void ya_list_init(void *node);
int ya_list_empty(const void *node);
int ya_list_original(const void *node);
int ya_list_size(const void *list);
void ya_list_erase(void *node);
void * ya_list_search(void *list, void *value,
	       		int (*comp)(void *value, const void *node));
void *ya_list_get(const void *list, int pos);
void ya_list_insert_after(void *pos, void *node);
int ya_list_add(void *list, void *node, int pos);

#endif
