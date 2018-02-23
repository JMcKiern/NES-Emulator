#include "Peripheral.h"

void Peripheral::UpdatePeripheral() {
	throw std::exception("Error: UpdatePeripheral() must be overriden before use.");
}

Peripheral::Peripheral(CPU* _cpu, uint16_t _addr, bool _isIRQConnection) {
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