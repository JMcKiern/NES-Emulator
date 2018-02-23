#pragma once

#ifdef _DEBUG_MODE
	#include <iostream>
#endif
#include <cstdint>
#include "BitFlag.h"
#include "MirrorManager.h"

// TODO:
//		Add error if Read before Write

class CPUMem {
private:
	union {
		uint8_t mem[0x10000];
		struct
		{
			uint8_t RAM[0x2000];
			uint8_t IORegisters[0x2200];
			uint8_t ExpansionRom[0x1E00];
			uint8_t SRAM[0x2000];
			uint8_t PRGROM[0x2000];
		};
	};
	MirrorManager mm;
	void SetupMirrors();

public:
	void Write(uint16_t offset, uint8_t data);
	uint8_t Read(uint16_t offset);

	CPUMem();
	CPUMem(bool shouldSetupBlank);
};