#include "UxROM.h"

void UxROM::Write(uint16_t addr, uint8_t data) {
	if (0x8000 <= addr && addr < 0x10000) {
		uint8_t bankNum = data & 0x7;
		PRGROML = GetPtrPRG(bankNum, 0x4000);
	}
}

UxROM::UxROM(std::ifstream& f) :
	Mapper(f)
{
	PRGROML = GetPtrPRG(0, 0x4000);
	PRGROMU = GetPtrPRG(num16kPRGBanks - 1, 0x4000);
	if (num8kCHRBanks > 0) {
		CHRROM = GetPtrCHR(0, 0x2000);
	}
	else {
		CHRRAM = new uint8_t[0x2000];
	}
}