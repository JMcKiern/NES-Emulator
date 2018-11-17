#include "RegisterInterrupt.h"

void RegisterInterrupt::UpdatePeripheral() {
	if (((cpu->PeripheralRead(addr) >> bitNum) & 0x1))
		peripheralConnection.SetState(LOW);
	else
		peripheralConnection.SetState(HIGH);
}

RegisterInterrupt::RegisterInterrupt(CPU_6502* _cpu, uint16_t _addr, bool _isIRQConnection, uint8_t _bitNum) :
	Peripheral(_cpu, _addr, _isIRQConnection),
	bitNum(_bitNum)
{}