#include "hook.h"

bool CHook::set_hook(tAttached process, const char * func, const char * dll)
{
	// Resolve export address of target function
	void * address = (void*)GetProcAddress(GetModuleHandleA(dll), func);

	if (!address)
	{
		dbg.print(LOG_FAILED, " - Failed to resolve %s!%s\n", dll, func);
		return false;
	}

	BYTE jmp[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

	size_t size = get_stub_size((BYTE*)address);

	struct tDisasm * p_disasm = disasm.disassemble((BYTE*)address, 5);

	return false;
}

bool CHook::rem_hook(tAttached process, const char * func, const char * dll)
{
	return false;
}

size_t CHook::get_stub_size(BYTE * buffer)
{
	size_t size = 0;

	while (buffer[size] != 0xC2 && buffer[size] != 0xC3)
		size++;

	if (*(buffer + size) == 0xC2)
		size += sizeof(WORD); // usually 2 bytes will follow the ret instruction

	return size;
}
