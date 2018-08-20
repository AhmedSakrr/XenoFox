#pragma once

#include "includes.h"

#define PATCH_SIZE	5
#define JMP_SIZE	5

class CHook
{
public:

	// init hooking space
	void init_space(int numberOfFunctionsToHook);

	// installs an inline hook
	bool set_hook(struct tAttached process, void *proxy, const char *func, const char *dll);

	// uninstalls an inline hook
	bool rem_hook(struct tAttached process, void *proxy, const char *func, const char *dll);

	// tries to get the size of a function stub by scanning for "ret" 
	size_t get_stub_size(BYTE *buffer) const;

private:
	
	// uses memcpy internally. Uses m_Position as destination offset relative to the destination base. Also increases m_Position by size.
	void copymem(void *dest, const void *src, size_t size);

	// checks whether the memory location is writable or not.
	bool is_writeable(void *addr, size_t size);

	void *m_ptrSpace = nullptr;
	int m_Position = 0;
};

extern CHook hook;