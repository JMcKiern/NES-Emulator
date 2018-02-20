#pragma once

#include <string>
#include <cstdint>
#include "enums.h"

class CPU;
typedef void (CPU::*OpFnPtr)(uint16_t offset, AddrMode addrMode);

struct Operation {
	Instr instr;
	std::string instrStr;
	AddrMode addrMode;
	std::string addrModeStr;
	OpFnPtr Run;
	uint8_t numCycles;

	Operation(Instr _instr, std::string _instrStr, AddrMode _addrMode, std::string _addrModeStr, OpFnPtr _Run, uint8_t _numCycles) :
		instr(_instr),
		instrStr(_instrStr),
		addrMode(_addrMode),
		addrModeStr(_addrModeStr),
		Run(_Run),
		numCycles(_numCycles)
	{}
	Operation() :
		instr(INSTR_ILLEGAL),
		instrStr("ILLEGAL"),
		addrMode(AM_ILLEGAL),
		addrModeStr("ILLEGAL"),
		Run(NULL),
		numCycles(numCycles)
	{}
};