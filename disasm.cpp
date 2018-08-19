#include "disasm.h"

CDisasm::CDisasm()
{
	if (cs_open(CS_ARCH_X86, CS_MODE_32, &this->m_handle) != ERROR_SUCCESS)
		throw "capstone handle";
}

CDisasm::~CDisasm()
{
	cs_close(&this->m_handle);
}

bool CDisasm::disassemble(BYTE * bytes, size_t size)
{
	this->m_count = cs_disasm(this->m_handle, bytes, size, 4096, size, &this->m_ptrInsn);

	if (this->m_count > 0)
	{


		this->m_count = 0;
	}

	return false;
}
