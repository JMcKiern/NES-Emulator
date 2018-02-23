#include "RegisterInterrupt.h"

void RegisterInterrupt::UpdatePeripheral() {
	peripheralConnection.SetState(((cpu->PeripheralRead(addr) >> bitNum) & 0x1));
}

RegisterInterrupt::RegisterInterrupt(CPU* _cpu, uint16_t _addr, bool _isIRQConnection, uint8_t _bitNum) :
	Peripheral(_cpu, _addr, _isIRQConnection),
	bitNum(_bitNum)
{}