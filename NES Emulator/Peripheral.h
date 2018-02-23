#pragma once

#include "CPU.h"
#include "PeripheralConnection.h"

class Peripheral {
private:

protected:
	CPU* cpu;
	PeripheralConnection peripheralConnection;
	uint16_t addr;
	bool isIRQConnection;

public:
	virtual void UpdatePeripheral() = 0;

	Peripheral(CPU* _cpu, uint16_t _writeAddr, bool _isIRQConnection);
	~Peripheral();
};