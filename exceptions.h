#pragma once

#include "includes.h"

class CException
{
public:
	int handle(int code, PEXCEPTION_POINTERS ex);
};

extern CException exception;