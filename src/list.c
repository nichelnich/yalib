#include <stdio.h>
#include <stdlib.h>
#include "ya/list.h"

void ya_list_init(void *node)
{
	ya_list_node_t *node_i = (ya_list_node_t *)node;
	node_i->prev = node;
	node_i->next = node;
}
