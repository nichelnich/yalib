/*
 * brief
 */

#ifndef __LOG_H__
#define __LOG_H__

enum log_level {
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
};

void ya_log(int level, const char *format, ...);

#endif
