#ifndef OPERATION_H
#define	OPERATION_H

#include "enums.h"

struct Operation {
	Instr instr;
	AddrMode addrMode;
	Operation(Instr _instr, AddrMode _addrMode) : instr(_instr), addrMode(_addrMode) {}
	Operation() : instr(INSTR_ILLEGAL), addrMode(AM_ILLEGAL) {}
};

#endif