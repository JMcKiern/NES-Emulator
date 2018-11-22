#include "CNROM.h"

void CNROM::Write(uint16_t addr, uint8_t data) {
	if (0x8000 <= addr && addr < 0x10000) {
		uint8_t bankNum = data & 0x3;
		CHRROM = GetPtrCHR(bankNum);
	}
}
CNROM::CNROM(std::ifstream& f) :
	Mapper(f)
{
	PRGROML = GetPtrPRG(0);
	if (num16kPRGBanks != 1) {
		PRGROMU = GetPtrPRG(num16kPRGBanks - 1);
	}
	else {
		PRGROMU = GetPtrPRG(0);
	}
	CHRROM = GetPtrCHR(0);
}