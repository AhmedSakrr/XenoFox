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
#include "ntapi.h"
#include "log.h"
#include "disasm.h"
