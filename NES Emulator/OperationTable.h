#ifndef OPERATIONTABLE_H
#define OPERATIONTABLE_H

#include <cstdint>
#include "Operation.h"

class CPU;

class OperationTable {
private:
	Operation operationTable[0xFF];
	void SetupOperationTable();

public:
	Operation GetOperation(uint8_t opcode);
	void LinkFunction(Instr instr, void(CPU::*_Run)(uint16_t offset, AddrMode addrMode));

	OperationTable();
};

#endif