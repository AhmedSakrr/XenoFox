#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <winternl.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include "status_codes.h"
#include "hook.h"
#include "process.h"
#include "system.h"
#include "log.h"
#include "disasm.h"
#include "exceptions.h"

// Throws an exception if the condition is not met
extern void assert(bool condition, const char *exception);

#include "ntapi.h"