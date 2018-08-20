#include "hook.h"

void CHook::init_space(int numberOfFunctionsToHook)
{
	if (this->m_ptrSpace != nullptr)
		VirtualFree(this->m_ptrSpace, 0, MEM_RELEASE);

	this->m_ptrSpace = VirtualAlloc(NULL, numberOfFunctionsToHook * (PATCH_SIZE * JMP_SIZE), MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	assert(this->m_ptrSpace != nullptr, "init_space");

	this->m_Position = 0;
}

bool CHook::set_hook(tAttached process, void *dest, const char * func, const char * dll)
{
	assert(this->m_ptrSpace != nullptr, "set_hook: init_space is null");

	// Resolve export address of target function
	void * address = (void*)GetProcAddress(GetModuleHandleA(dll), func);

	if (!address)
	{
		dbg.print(LOG_FAILED, " - Failed to resolve %s!%s\n", dll, func);
		return false;
	}

	BYTE jmp[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	BYTE jmpBack[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

	size_t size = get_stub_size((BYTE*)address);

	struct tDisasm * p_disasm = disasm.disassemble((BYTE*)address, 5);

	if (p_disasm == nullptr)
	{
		dbg.print(LOG_FAILED, "  - Failed to hook \"%s!%s\n for process (%d)\n", dll, func, process.id);
		return false;
	}

	// Copy original 5 bytes into our buffer
	copymem(this->m_ptrSpace, p_disasm->data, p_disasm->size);

	// Build jmp instruction to jmp back to the original function
	// Important: calculate the target jmp location by skipping the first 5 bytes (trampoline!)
	// dest - src
	*(DWORD*)(jmpBack + 1) = ((DWORD)address + p_disasm->size + 5) - ((DWORD)this->m_ptrSpace);
	
	// Copy jmp instruction into our buffer
	copymem(this->m_ptrSpace, (LPCVOID)jmpBack, sizeof(jmpBack));

	// Build jmp instruction to jmp on our hooked function
	*(DWORD*)(jmp + 1) = (DWORD)dest - (DWORD)address;

	// Patch stub
	memcpy(address, (LPCVOID)jmp, sizeof(jmp));

	return false;
}

bool CHook::rem_hook(tAttached process, void *dest, const char * func, const char * dll)
{
	return false;
}

size_t CHook::get_stub_size(BYTE * buffer) const
{
	size_t size = 0;

	while (buffer[size] != 0xC2 && buffer[size] != 0xC3)
		size++;

	if (*(buffer + size) == 0xC2)
		size += sizeof(WORD); // RET imm16 - Near return to the calling procedure inside the current code segment - pops 2 bytes from stack. This is required.

	return size;
}

void CHook::copymem(void * dest, const void * src, size_t size)
{
	memcpy((void*)((DWORD)dest + this->m_Position), src, size);
	this->m_Position += size;
}
