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

tDisasm *CDisasm::disassemble(BYTE * bytes, size_t size)
{
	struct tDisasm * p_disasm= (tDisasm*)malloc(sizeof(tDisasm));

	assert(p_disasm, "memory allocation failed");

	RtlSecureZeroMemory(p_disasm, sizeof(tDisasm));

	this->m_count = cs_disasm(this->m_handle, bytes, size, 0x1000, size, &this->m_ptrInsn);

	if (this->m_count > 0)
	{
		// get correct length
		for (int i = 0; i < this->m_count && p_disasm->size < size; i++)
		{
			cs_insn * ins = &(this->m_ptrInsn[i]);
			p_disasm->size += ins->size;
		}

		assert(p_disasm->size >= 5, "disassembling 5 bytes failed");

		// allocate block to insert instructions which have a length 5 bytes or greater
		p_disasm->data = (BYTE*)malloc(p_disasm->size);

		// fill 5 bytes
		int dataIndex = 0;
		int bytesCopied = 0;
		for (int i = 0; i < this->m_count; i++)
		{
			cs_insn * ins = &(this->m_ptrInsn[i]);

			for (int ii = 0; ii < ins->size; ii++) 
				p_disasm->data[dataIndex++] = ins->bytes[ii];

			bytesCopied += ins->size;

			if (bytesCopied >= p_disasm->size)
				break;
		}

		cs_free(this->m_ptrInsn, this->m_count);
		this->m_count = 0;

		return p_disasm;
	}

	return nullptr;
}

void CDisasm::disasm_free(void * ptr)
{
	if (ptr == nullptr)
		return;

	tDisasm * p_disasm = (tDisasm*)ptr;

	if (p_disasm->data != nullptr) {
		free(p_disasm);
		p_disasm->data = nullptr;
	}

	free(p_disasm);
}

