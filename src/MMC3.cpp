#include "MMC3.h"
#include "CPU_NES.h"

bool MMC3::UsingCHRROM() {
	return num8kCHRBanks > 0;
}

uint8_t MMC3::PPURead(uint16_t addr) {
	if (num8kCHRBanks > 0) {
		int bankNum = addr / 0x400;
		int offset = addr % 0x400;
		return CHRROM[bankNum][offset];
	}
	else
		return CHRRAM[addr];
}
void MMC3::IRQClock() {
	if (irqConnection.GetState() == LOW && irqCycle < cpuPtr->GetTotalCycles()) {
		// Ensure that IRQ is low for at least one cpu tick (i.e. so it can be
		// detected)
		irqConnection.SetState(HIGH);
	}
	if ((irqCounter == 0) || irqReloadFlag) {
		irqCounter = irqLatch;
	}
	else {
		irqCounter--;
		// Normal/new behaviour
		if (irqCounter == 0 && irqEnabled) {
			irqConnection.SetState(LOW);
			irqCycle = cpuPtr->GetTotalCycles();
		}
	}
	/*// Alternate/new behaviour
	if (irqCounter == 0 && irqEnabled) {
		irqConnection.SetState(LOW);
	}*/
}
void MMC3::Write(uint16_t addr, uint8_t data) {
	if (0x8000 <= addr && addr <= 0x9FFF) {
		if (addr % 2 == 0) {
			uint8_t RRR = data & 0x3;
			uint8_t P = (data >> 6) & 0x1;
			uint8_t C = (data >> 7) & 0x1;

			nextBank = RRR;

			PRGROMBankMode = P == 0 ? PRGROM_SWAPPABLE_8000
			                        : PRGROM_SWAPPABLE_C000;
			CHRInversion = C == 1;

		}
		else {
			switch (nextBank) {
			case 0: {
				int baseBankNum4K = (data * 2);
				if (!CHRInversion) {
					CHRROM[0] = GetPtrCHR(baseBankNum4K, 0x0400);
					CHRROM[1] = GetPtrCHR(baseBankNum4K + 1, 0x0400);
				}
				else {
					CHRROM[4] = GetPtrCHR(baseBankNum4K, 0x0400);
					CHRROM[5] = GetPtrCHR(baseBankNum4K + 1, 0x0400);
				}
				break;
			}
			case 1: {
				int baseBankNum4K = (data * 2);
				if (!CHRInversion) {
					CHRROM[2] = GetPtrCHR(baseBankNum4K, 0x0400);
					CHRROM[3] = GetPtrCHR(baseBankNum4K + 1, 0x0400);
				}
				else {
					CHRROM[6] = GetPtrCHR(baseBankNum4K, 0x0400);
					CHRROM[7] = GetPtrCHR(baseBankNum4K + 1, 0x0400);
				}
				break;
			}
			case 2: {
				int baseBankNum4K = data;
				if (!CHRInversion) {
					CHRROM[4] = GetPtrCHR(baseBankNum4K, 0x0400);
				}
				else {
					CHRROM[0] = GetPtrCHR(baseBankNum4K, 0x0400);
				}
				break;
			}
			case 3: {
				int baseBankNum4K = data;
				if (!CHRInversion) {
					CHRROM[5] = GetPtrCHR(baseBankNum4K, 0x0400);
				}
				else {
					CHRROM[1] = GetPtrCHR(baseBankNum4K, 0x0400);
				}
				break;
			}
			case 4: {
				int baseBankNum4K = data;
				if (!CHRInversion) {
					CHRROM[6] = GetPtrCHR(baseBankNum4K, 0x0400);
				}
				else {
					CHRROM[2] = GetPtrCHR(baseBankNum4K, 0x0400);
				}
				break;
			}
			case 5: {
				int baseBankNum4K = data;
				if (!CHRInversion) {
					CHRROM[7] = GetPtrCHR(baseBankNum4K, 0x0400);
				}
				else {
					CHRROM[3] = GetPtrCHR(baseBankNum4K, 0x0400);
				}
				break;
			}
			case 6: {
				uint8_t bankNum = data & ~0xc0;
				if (PRGROMBankMode == PRGROM_SWAPPABLE_8000) {
					PRGROM[0] = GetPtrPRG(bankNum, 0x1000);
				}
				else {
					PRGROM[2] = GetPtrPRG(bankNum, 0x1000);
				}
				break;
			}
			case 7: {
				uint8_t bankNum = data & ~0xc0;
				PRGROM[1] = GetPtrPRG(bankNum, 0x1000);
				break;
			}
			default:
				break;
			}
		}
	}
	else if (0xA000 <= addr && addr <= 0xBFFF) {
		if (addr % 2 == 0) {
			// Mirroring
			uint8_t MM = data & 0x1;
			switch (MM) {
				case 0: mirroringMode = VERTICAL; break;
				case 1: mirroringMode = HORIZONTAL; break;
			}
		}
		else {
			// PRG RAM Protect

			// Many implementations of iNES Mapper 004 simply omit the write
			// protection. 
			uint8_t writeProtect = (data >> 6) & 0x1;

			isPRGRAMEnabled = (data >> 7) & 0x1;
		}
	}
	else if (0xC000 <= addr && addr <= 0xDFFF) {
		if (addr % 2 == 0) {
			// IRQ Latch
			irqLatch = data;
		}
		else {
			// IRQ Reload
			irqReloadFlag = true;
		}
	}
	else if (0xE000 <= addr && addr <= 0xFFFF) {
		irqEnabled = addr % 2 == 0;
	}
}

uint8_t MMC3::Read(uint16_t addr) {
	if (0x6000 <= addr && addr < 0x8000) {
		if (PRGRAM != NULL) {
			if (isPRGRAMEnabled)
				return PRGRAM[addr - 0x6000];
			else
				return cpuPtr->openBus;
		}
		else
			return 0;
	}
	else if (0x8000 <= addr && addr < 0xC000) {
		if (PRGROML != NULL)
			return PRGROML[addr - 0x8000];
		else
			return 0;
	}
	else if (0xC000 <= addr && addr < 0x10000) {
		if (PRGROMU != NULL)
			return PRGROMU[addr - 0xC000];
		else
			return 0;
	}
	else {
		// From Ice Climbers it looks like returning any value here is ok
		return 0;
	}
	//throw MemoryAddressNotValidException();
}

MMC3::MMC3(std::ifstream& f, CPU_NES* cpuPtr) :
	Mapper(f)
{
	if (num8kRAMBanks > 0)
		PRGRAM = new uint8_t[0x2000];
	PRGROML = GetPtrPRG(0, 0x4000);
	PRGROMU = GetPtrPRG(num16kPRGBanks - 1, 0x4000);
	if (num8kCHRBanks > 0) {
		//CHRROML = GetPtrCHR(0, 0x1000);
		//CHRROMU = GetPtrCHR(1, 0x1000);
	}
	else {
		CHRRAM = new uint8_t[0x2000];
	}
	cpuPtr->AddIRQConnection(&irqConnection);
}