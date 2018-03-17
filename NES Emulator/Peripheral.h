#pragma once

#include "CPU_6502.h"
#include "PeripheralConnection.h"

class Peripheral {
private:

protected:
	CPU_6502* cpu;
	PeripheralConnection peripheralConnection;
	uint16_t addr;
	bool isIRQConnection;

public:
	virtual void UpdatePeripheral() = 0;

	Peripheral(CPU_6502* _cpu, uint16_t _writeAddr, bool _isIRQConnection);
	~Peripheral();
};