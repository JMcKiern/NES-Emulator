#pragma once

#include <cstdint>
#include <string>
#include "enums.h"

class CPU_6502;
typedef void (CPU_6502::*OpFnPtr)(uint16_t offset, AddrMode addrMode);

struct Operation {
	Instr instr;
	std::string instrStr;
	AddrMode addrMode;
	std::string addrModeStr;
	OpFnPtr Run;
	uint8_t numCycles;

	Operation(Instr _instr, const std::string& _instrStr, AddrMode _addrMode,
	          const std::string& _addrModeStr, OpFnPtr _Run,
			  uint8_t _numCycles) :
		instr(_instr),
		instrStr(_instrStr),
		addrMode(_addrMode),
		addrModeStr(_addrModeStr),
		Run(_Run),
		numCycles(_numCycles)
	{}
	Operation() :
		instr(INSTR_ILLEGAL),
		instrStr("INSTR_ILLEGAL"),
		addrMode(AM_ILLEGAL),
		addrModeStr("AM_ILLEGAL"),
		Run(NULL),
		numCycles(0xff)
	{}
};