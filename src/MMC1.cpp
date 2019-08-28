#include "MMC1.h"

bool MMC1::UsingCHRROM() {
	return num8kCHRBanks > 0;
}

uint8_t MMC1::PPURead(uint16_t addr) {
	if (num8kCHRBanks > 0) {
		if (addr < 0x1000)
			return CHRROML[addr];
		else
			return CHRROML[addr];
	}
	else
		return CHRRAM[addr];
}
void MMC1::Write(uint16_t addr, uint8_t data) {
	if (0x8000 <= addr && addr <= 0xFFFF) {
		//shiftReg = (shiftReg << 1) | (data & 1);
		shiftReg = (shiftReg >> 1) | ((data & 1) << 4);
		shiftNum++;

		if (((data >> 7) & 1) == 1) {
			shiftReg = 0;
			shiftNum = 0;
		}
		if (shiftNum == 5) {
			if (0x8000 <= addr && addr <= 0x9FFF) {
				// Control
				uint8_t MM = shiftReg & 0x3;
				switch (MM) {
				case 0: mirroringMode = ONE_SCREEN_LOWER_BANK; break;
				case 1: mirroringMode = ONE_SCREEN_UPPER_BANK; break;
				case 2: mirroringMode = VERTICAL; break;
				case 3: mirroringMode = HORIZONTAL; break;
				}
				uint8_t PP = (shiftReg >> 2) & 0x3;
				switch (PP) {
				case 0:
				case 1: PRGROMBankMode = PRGROM_32; break;
				case 2: PRGROMBankMode = PRGROM_16U; break;
				case 3: PRGROMBankMode = PRGROM_16L; break;
				}
				uint8_t C = (shiftReg >> 4) & 0x1;
				shouldSwitch8KCHRROM = C == 0;
			}
			else if (0xA000 <= addr && addr <= 0xBFFF) {
				// CHR bank 0
				uint8_t bankNum = (shiftReg & 0x1F);
				if (shouldSwitch8KCHRROM) bankNum = bankNum & 0x1;
				if (UsingCHRROM()) {
					CHRROML = GetPtrCHR(bankNum, 0x1000);
				}
				else {
					//CHRRAML = GetPtrCHR(bankNum, 0x1000);
				}
			}
			else if (0xC000 <= addr && addr <= 0xDFFF) {
				// CHR bank 1
				if (!shouldSwitch8KCHRROM) {
					uint8_t bankNum = (shiftReg & 0x1F);
					if (UsingCHRROM()) {
						CHRROMU = GetPtrCHR(bankNum, 0x2000);
					}
					else {
						//CHRRAMU = GetPtrCHR(bankNum, 0x1000);
					}
				}
			}
			else if (0xE000 <= addr && addr <= 0xFFFF) {
				// PRG bank
				uint8_t PPPP = shiftReg & 0xF;
				switch (PRGROMBankMode) {
				case PRGROM_32:
					PRGROML = GetPtrPRG(PPPP, 0x4000);
					PRGROMU = GetPtrPRG(PPPP + 1, 0x4000);
					break;
				case PRGROM_16U:
					PRGROMU = GetPtrPRG(PPPP, 0x4000);
					break;
				case PRGROM_16L:
					PRGROML = GetPtrPRG(PPPP, 0x4000);
					break;
				}
				uint8_t R = (shiftReg >> 4) & 0x1;
				if (R == 0) {
					if (PRGRAM == nullptr) {
						// https://stackoverflow.com/a/2030018/2471379
						PRGRAM = new uint8_t[0x2000]();
					}
				}
				else {
					delete[] PRGRAM;
					PRGRAM = nullptr;
				}
			}
			shiftReg = 0;
			shiftNum = 0;
		}
	}

}

MMC1::MMC1(std::ifstream& f) :
	Mapper(f)
{
	if (num8kRAMBanks > 0)
		PRGRAM = new uint8_t[0x2000];
	PRGROML = GetPtrPRG(0, 0x4000);
	PRGROMU = GetPtrPRG(num16kPRGBanks - 1, 0x4000);
	if (num8kCHRBanks > 0) {
		CHRROML = GetPtrCHR(0, 0x1000);
		CHRROMU = GetPtrCHR(1, 0x1000);
	}
	else {
		CHRRAM = new uint8_t[0x2000];
	}
}