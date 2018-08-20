#pragma once

#include "includes.h"


class CHook
{
public:

	// installs an inline hook
	bool set_hook(struct tAttached process, const char *func, const char *dll);

	// uninstalls an inline hook
	bool rem_hook(struct tAttached process, const char *func, const char *dll);

	// tries to get the size of a function stub by scanning for "ret" 
	size_t get_stub_size(BYTE *buffer);
};

extern CHook hook;