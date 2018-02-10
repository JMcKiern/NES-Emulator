#ifndef OPERATION_H
#define	OPERATION_H

#include <string>
#include "enums.h"

class CPU;
typedef void (CPU::*OpFnPtr)(uint16_t offset, AddrMode addrMode);

struct Operation {
	Instr instr;
	std::string instrStr;
	AddrMode addrMode;
	std::string addrModeStr;
	OpFnPtr Run;
	bool shouldIncPC;

	Operation(Instr _instr, std::string _instrStr, AddrMode _addrMode, std::string _addrModeStr, OpFnPtr _Run, bool _shouldIncPC) :
		instr(_instr),
		instrStr(_instrStr),
		addrMode(_addrMode),
		addrModeStr(_addrModeStr),
		Run(_Run),
		shouldIncPC(_shouldIncPC)
	{}
	Operation() :
		instr(INSTR_ILLEGAL),
		instrStr("ILLEGAL"),
		addrMode(AM_ILLEGAL),
		addrModeStr("ILLEGAL"),
		Run(NULL),
		shouldIncPC(true)
	{}
};

#endif