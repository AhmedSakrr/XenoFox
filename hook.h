#pragma once

#include "includes.h"


class CHook
{
public:

	bool set_hook(struct tAttached process, const char *func, const char *dll);

};

extern CHook hook;