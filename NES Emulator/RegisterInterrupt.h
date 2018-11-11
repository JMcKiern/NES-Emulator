#include "Peripheral.h"

class RegisterInterrupt : public Peripheral {
private:
	uint8_t bitNum;

public:
	void UpdatePeripheral() override;
	RegisterInterrupt(CPU_6502* _cpu, uint16_t _addr, bool _isIRQConnection,
	                  uint8_t _bitNum);
};
