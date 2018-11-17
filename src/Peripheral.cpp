#include "Peripheral.h"

Peripheral::Peripheral(CPU_6502* _cpu, uint16_t _addr, bool _isIRQConnection) {
	cpu = _cpu;
	isIRQConnection = _isIRQConnection;
	if (isIRQConnection) {
		cpu->AddIRQConnection(&peripheralConnection);
	}
	else {
		cpu->AddNMIConnection(&peripheralConnection);
	}
	addr = _addr;
}
Peripheral::~Peripheral() {
	if (isIRQConnection) {
		cpu->RemoveIRQConnection(&peripheralConnection);
	}
	else {
		cpu->RemoveNMIConnection(&peripheralConnection);
	}
}