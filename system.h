#pragma once

#include "includes.h"

enum eArchitecture
{
	OS_X86,
	OS_X64
};

class CSystem
{
public:
	eArchitecture GetArchitecture();
};

extern CSystem os;