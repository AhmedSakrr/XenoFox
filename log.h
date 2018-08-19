#pragma once

#include "includes.h"

#include <stdarg.h>
#include <stdio.h>

enum eLogType
{
	LOG_SUCCESS,
	LOG_FAILED,
	LOG_WARNING
};

class CLog
{
public:
	CLog();
	~CLog();

	void print(eLogType type, const char *fmt, ...);

private:
	void set_color(eLogType type);
	void restore_color();

	HANDLE console_handle = INVALID_HANDLE_VALUE;
	CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer = { 0 };

}; 

extern CLog dbg;