/*
 * brief
 */

#include <assert.h>

void ya_debug_assert(scalar expression) /*boolen*/
{
#ifdef DEBUG_ASSERT
	assert(expression);
#endif
}

