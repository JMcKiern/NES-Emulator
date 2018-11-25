#pragma once

#include <fstream>
#include "Mapper.h"

class MMC1 : public Mapper {
	uint8_t* CHRROML;
	uint8_t* CHRROMU;
	uint8_t shiftReg = 0;
	int shiftNum = 0;
	bool shouldSwitch8KCHRROM;
	bool UsingCHRROM();
	enum {
		PRGROM_32,
		PRGROM_16U,
		PRGROM_16L
	};
	int PRGROMBankMode = PRGROM_16L; // Castlevania

public:
	uint8_t PPURead(uint16_t addr);
	void Write(uint16_t addr, uint8_t data);
	MMC1(std::ifstream& f);
};