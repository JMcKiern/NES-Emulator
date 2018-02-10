#ifndef OPERATIONTABLE_H
#define OPERATIONTABLE_H

#include <cstdint>
#include "Operation.h"

class OperationTable {
private:
	Operation operationTable[0xFF];
	void SetupOperationTable();

public:
	Operation GetOperation(uint8_t opcode);

	OperationTable();
};

#endif