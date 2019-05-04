#include "MMC3.h"
#include "CPU_NES.h"

bool MMC3::UsingCHRROM() {
	return num8kCHRBanks > 0;
}

uint8_t MMC3::PPURead(uint16_t addr) {
	if (num8kCHRBanks > 0) {
		int bankNum = addr / 0x400;
		int offset = addr % 0x400;

		if (CHRInversion) {
			bankNum = (bankNum + 4) % 8;
		}
		switch (bankNum) {
		case 0:
			return R[0][offset];
		case 1:
			return R[0][offset + 0x400];
		case 2:
			return R[1][offset];
		case 3:
			return R[1][offset + 0x400];
		case 4:
		case 5:
		case 6:
		case 7:
			return R[bankNum - 2][offset];
		default:
			return 0;
		}
	}
	else
		return CHRRAM[addr];
}
void MMC3::IRQClock() {
	if (!irqEnabled) {
		irqConnection.SetState(HIGH);
	}
	if ((irqCounter == 0) || irqReloadFlag) {
		irqCounter = irqLatch;
		irqReloadFlag = false;
	}
	else {
		irqCounter--;
		// Normal/new behaviour
		if (irqCounter == 0 && irqEnabled) {
			irqConnection.SetState(LOW);
		}
	}
	/*// Alternate/new behaviour
	if (irqCounter == 0 && irqEnabled) {
		irqConnection.SetState(LOW);
	}*/
}
void MMC3::Write(uint16_t addr, uint8_t data) {
	if (0x6000 <= addr && addr <=0x7FFF) {
		if (isPRGRAMEnabled && PRGRAM != NULL) {
			PRGRAM[addr - 0x6000] = data;
		}
	}
	else if (0x8000 <= addr && addr <= 0x9FFF) {
		if (addr % 2 == 0) {
			uint8_t RRR = data & 0x7;
			uint8_t P = (data >> 6) & 0x1;
			uint8_t C = (data >> 7) & 0x1;

			nextBankRegister = RRR;

			PRGROMBankMode = P == 0 ? PRGROM_SWAPPABLE_8000
			                        : PRGROM_SWAPPABLE_C000;
			CHRInversion = C == 1;

		}
		else {
			uint8_t bankNum = data;
			switch (nextBankRegister) {
			case 0:
			case 1: {
				bankNum &= ~0x01;
				R[nextBankRegister] = GetPtrCHR(bankNum, 0x400);
				break;
			}
			case 2:
			case 3:
			case 4:
			case 5:{
				R[nextBankRegister] = GetPtrCHR(bankNum, 0x400);
				break;
			}
			case 6:
			case 7: {
				bankNum &= ~0xc0;
				R[nextBankRegister] = GetPtrPRG(bankNum, 0x2000);
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
		irqEnabled = addr % 2 == 1;
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
	else if (0x8000 <= addr && addr < 0x10000) {
		addr = addr - 0x8000;
		int bankNum = addr / 0x2000;
		int offset = addr % 0x2000;

		switch (bankNum) {
		case 0:
			if (PRGROMBankMode == PRGROM_SWAPPABLE_8000) {
				return R[6][offset];
			}
			else {
				return GetPtrPRG((2 * num16kPRGBanks) - 2, 0x4000 / 2)[offset];
			}
		case 1:
			return R[7][offset];
		case 2:
			if (PRGROMBankMode == PRGROM_SWAPPABLE_8000) {
				return GetPtrPRG((2 * num16kPRGBanks) - 2, 0x4000 / 2)[offset];
			}
			else {
				return R[6][offset];
			}
		case 3:
			return GetPtrPRG((2 * num16kPRGBanks) - 1, 0x4000 / 2)[offset];
		default:
			return 0;
		}
	}
	else {
		// From Ice Climbers it looks like returning any value here is ok
		return 0;
	}
	//throw MemoryAddressNotValidException();
	
}

MMC3::MMC3(std::ifstream& f, CPU_NES* _cpuPtr) :
	Mapper(f)
{
	PRGRAM = new uint8_t[0x2000];
	if (num8kCHRBanks > 0) {
		//CHRROML = GetPtrCHR(0, 0x1000);
		//CHRROMU = GetPtrCHR(1, 0x1000);
	}
	else {
		CHRRAM = new uint8_t[0x2000];
	}
	for (int i = 0; i < 6; i++) {
		// Setting this because it will be accessed before it can be set by
		// the loaded rom.
		R[i] = GetPtrCHR(0);
		// Not setting PRG since that would have consequences if it was loaded
		// wrong. The software in the last prgrom bank should load this on
		// reset.
	}
	cpuPtr = _cpuPtr;
	cpuPtr->AddIRQConnection(&irqConnection);
}