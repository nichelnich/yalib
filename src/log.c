/*
 * brief
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ya/log.h"


void ya_log(int level, const char *format, ...)
{
	va_list arg;
	va_start(arg, format);
	printf(format, arg);
	va_end(arg);
}
