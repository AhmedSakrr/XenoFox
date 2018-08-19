#pragma once

#include "includes.h"


class CHook
{
public:

	bool SetHook(struct tAttached process, const char *func, const char *dll);

};

extern CHook hook;