#ifndef _PPU_H
#define _PPU_H

#include <cstdint>
#include "PPUMem.h"


class PPU {
private:
	CPUMem* CPUMemPtr;
	uint8_t AccessPPUControlRegister();
	void SetPPUStatusRegister(uint8_t data);
public:
	PPU(CPUMem* _CPUMemPtr);
};

#endif // !_PPU_H

