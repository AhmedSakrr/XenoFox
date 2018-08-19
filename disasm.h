#pragma once

#include "includes.h"

// Capstone Dissasembler Engine
#include "capstone/include/capstone.h"

class CDisasm
{
public:
	CDisasm();
	~CDisasm();

	bool disassemble(BYTE *bytes, size_t size);

private:
	csh m_handle;
	cs_insn *m_ptrInsn;
	size_t m_count;
};

extern CDisasm disasm;