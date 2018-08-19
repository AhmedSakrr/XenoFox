#include "system.h"

eArchitecture CSystem::GetArchitecture()
{
	BOOL fIs64 = FALSE;
	
	if (IsWow64Process(GetCurrentProcess(), &fIs64))
	{
		return (fIs64) ? OS_X64 : OS_X86;
	}

	throw "IsWow64Process has failed.";
}
