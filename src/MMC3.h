#pragma once

#include <fstream>
#include "Mapper.h"
#include "CPU_NES.h"
#include "PeripheralConnection.h"

class MMC3 : public Mapper {
	uint8_t* R[8];
	//uint8_t shiftReg = 0;
	//int shiftNum = 0;
	bool shouldSwitch8KCHRROM;
	bool UsingCHRROM();
	enum {
		R0_CHR2K_0x0000,
		R1_CHR2K_0x0800,
		R2_CHR1K_0x1000,
		R3_CHR1K_0x1400,
		R4_CHR1K_0x1800,
		R5_CHR1K_0x1c00,
		R6_PRG8K_0x8000,
		R7_PRG8K_0xA000
	};
	int nextBankRegister = R0_CHR2K_0x0000;
	enum {
		PRGROM_SWAPPABLE_8000,
		PRGROM_SWAPPABLE_C000
	};
	int PRGROMBankMode = PRGROM_SWAPPABLE_8000;
	//int PRGROMBankMode = PRGROM_16L; // Castlevania
	enum {
		CHR_ADDR_INVERT_2x2KB_4x1KB,
		CHR_ADDR_INVERT_4x1KB_2x2KB,
	};
	bool CHRInversion = false;
	bool isPRGRAMEnabled = true;
	uint8_t irqLatch;
	uint8_t irqCounter;
	bool irqReloadFlag;
	bool irqEnabled;
	int irqCycle;
	CPU_NES* cpuPtr;
	PeripheralConnection irqConnection;

public:
	uint8_t PPURead(uint16_t addr);
	void IRQClock();
	void Write(uint16_t addr, uint8_t data);
	uint8_t Read(uint16_t addr);
	MMC3(std::ifstream& f, CPU_NES* _cpuPtr);
};