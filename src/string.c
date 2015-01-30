/*
 * brief
 */

#include <string.h>
#include <wchar.h>

char *ya_strcpy(char *dest, const char *src, size_t n)
{
	assert(dest && src); 

	memset(dest, 0, n);
	return strncpy(dest, src, n - 1);
}

wchar_t *ya_wcscpy(wchar_t *dest, const wchar_t *src, size_t n)
{
	assert(dest && src);

	memset(dest, 0, n * sizeof(wchar_t));
	return wcsncpy(dest, src, n - 1);
}

