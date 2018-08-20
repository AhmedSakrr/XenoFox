#pragma once

#include "includes.h"

// Capstone Dissasembler Engine
#include "capstone/include/capstone.h"

typedef struct tDisasm
{
	BYTE * data;
	size_t size;
};

class CDisasm
{
public:
	CDisasm();
	~CDisasm();

	tDisasm *disassemble(BYTE *bytes, size_t size);
	void disasm_free(void *ptr);

private:
	csh m_handle;
	cs_insn *m_ptrInsn;
	size_t m_count;
};

extern CDisasm disasm;