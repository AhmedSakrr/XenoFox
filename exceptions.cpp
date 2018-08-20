#include "exceptions.h"

int CException::handle(int code, PEXCEPTION_POINTERS ex)
{
	dbg.print(LOG_WARNING, " [!!!] EXCEPTION CAUGHT: %d (0x%x)", code, code);
	return EXCEPTION_SINGLE_STEP;
}
