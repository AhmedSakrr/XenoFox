#include "log.h"

CLog::CLog()
{
	this->console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!this->console_handle || this->console_handle == INVALID_HANDLE_VALUE)
		throw "Couldn't acquire console output handle";

	if (!GetConsoleScreenBufferInfo(this->console_handle, &this->console_screen_buffer))
		throw "Couldn't acquire console screen buffer information.";
}

CLog::~CLog()
{
	
}

void CLog::print(eLogType type, const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	set_color(type);

	vfprintf(stdout, fmt, args);

	restore_color();

	va_end(args);
}

void CLog::set_color(eLogType type)
{
	WORD color;

	switch (type)
	{
	case LOG_SUCCESS:
		color = FOREGROUND_GREEN;
		break;
	case LOG_FAILED:
		color = FOREGROUND_RED;
		break;
	case LOG_WARNING:
		color = FOREGROUND_GREEN|FOREGROUND_BLUE;
		break;
	}

	color |= FOREGROUND_INTENSITY;

	SetConsoleTextAttribute(this->console_handle, color);
}

void CLog::restore_color()
{
	SetConsoleTextAttribute(this->console_handle, this->console_screen_buffer.wAttributes);
}

