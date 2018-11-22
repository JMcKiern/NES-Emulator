#include "NROM.h"

NROM::NROM(std::ifstream& f) :
	Mapper(f)
{
	if (num16kPRGBanks == 1) {
		PRGROML = PRGROMU = GetPtrPRG(0);
	}
	else {
		PRGROML = GetPtrPRG(0);
		PRGROMU = GetPtrPRG(1);
	}
	if (num8kCHRBanks > 0) {
		CHRROM = GetPtrCHR(0);
	}
	else {
		CHRRAM = new uint8_t[0x2000];
	}
}