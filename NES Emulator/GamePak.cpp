#include <cstdint>
#include "GamePak.h"

void GamePak::LoadINes(std::string filename) {
	std::ifstream f;
	std::streampos sizef;

	f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
	sizef = f.tellg();
	f.seekg(0, std::ios::beg);

	size = static_cast<int>(sizef);
	cartridge = new char[size];

	f.read(cartridge, sizef);
	f.close();
	
	if(CheckINes()) {
		SetupINes();
	}
	else {
		throw InvalidFileTypeException();
	}
}
bool GamePak::CheckINes() {
	if (!(cartridge[0] == 'N' && cartridge[1] == 'E' && cartridge[2] == 'S' && cartridge[3] == 0x1A)) {
		delete[] cartridge;
		size = -1;
		return false;
	}
	return true;
}
uint16_t GamePak::GetOffsetPRG(uint8_t bankNum) {
	if (bankNum < 0 || bankNum >= num16kPRGBanks) {
		std::out_of_range::out_of_range("Attempted to access nonexistant bank number.");
	}
	uint16_t offset = 0x10; // Header
	if (isTrainer) offset += 0x200; // Trainer
	offset += 0x4000 * bankNum; // PRG Banks
	return offset;
}
uint16_t GamePak::GetOffsetCHR(uint8_t bankNum) {
	if (bankNum < 0 || bankNum >= num8kCHRBanks) {
		std::out_of_range::out_of_range("Attempted to access nonexistant bank number.");
	}
	uint16_t offset = 0x10; // Header
	if (isTrainer) offset += 0x200; // Trainer
	offset += 0x4000 * num16kPRGBanks; // PRG Banks
	offset += 0x2000 * bankNum; // CHR Banks
	return offset;
}
uint8_t* GamePak::GetPtrPRG(uint8_t bankNum) {
	uint16_t offset = GetOffsetPRG(bankNum);
	return reinterpret_cast<uint8_t*>(cartridge + offset);
}
uint8_t* GamePak::GetPtrCHR(uint8_t bankNum) {
	uint16_t offset = GetOffsetCHR(bankNum);
	return reinterpret_cast<uint8_t*>(cartridge + offset);
}
void GamePak::SetupINes() {
	num16kPRGBanks = cartridge[4];
	num8kCHRBanks = cartridge[5];
	usingVerticalMirroring = (cartridge[6] >> 0) & 0x1;
	isBatteryBackedRAM = (cartridge[6] >> 1) & 0x1;
	isTrainer = (cartridge[6] >> 2) & 0x1;
	usingFourScreenMirroring = (cartridge[6] >> 3) & 0x1;
	mapperNum = ((cartridge[6] >> 4) & 0xF) + (cartridge[7] & 0xF0);
	num8kRAMBanks = cartridge[8];

	switch (mapperNum) {
		case 0: {
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
			break;
		}
		case 2: {
			PRGROML = GetPtrPRG(0);
			PRGROMU = GetPtrPRG(num16kPRGBanks - 1);
			if (num8kCHRBanks > 0) {
				CHRROM = GetPtrCHR(0);
			}
			break;
		}
		case 3: {
			PRGROML = GetPtrPRG(0);
			if (num16kPRGBanks != 1) {
				PRGROMU = GetPtrPRG(num16kPRGBanks - 1);
			}
			else {
				PRGROMU = GetPtrPRG(0);
			}
			CHRROM = GetPtrCHR(0);
			break;
		}
		default: {
			throw MapperNotImplementedException();
			break;
		}
	}
}

void GamePak::RegisterUpdate(uint16_t addr, uint8_t data) {
	switch (mapperNum) {
		case 0: break;
		case 2: { // TODO: Bus conflicts?
			if (0x8000 <= addr && addr < 0x10000) {
				uint8_t bankNum = data & 0xF;
				PRGROMU = GetPtrPRG(bankNum);
			}
			break;
		}
		case 3: {
			if (0x8000 <= addr && addr < 0x10000) {
				uint8_t bankNum = data & 0x3; // TODO: check if 0x3 is correct
				CHRROM = GetPtrCHR(bankNum);
			}
			break;
		}
		default: {
			throw MapperNotImplementedException();
			break;
		}
	}
}
uint8_t GamePak::Read(uint16_t addr) {
	if (0x6000 <= addr && addr < 0x8000) {
		return PRGRAM[addr - 0x6000];
	}
	else if (0x8000 <= addr && addr < 0xC000) {
		return PRGROML[addr - 0x8000];
	}
	else if (0xC000 <= addr && addr < 0x10000) {
		return PRGROMU[addr - 0xC000];
	}
	throw MemoryAddressNotValidException();
}
void GamePak::Write(uint16_t addr, uint8_t data) {
	RegisterUpdate(addr, data);
}
bool GamePak::isVRAM(uint16_t addr) {
	// TODO: not sure if universally correct
	switch (mapperNum)
	{
		case 0:
		case 2:
		case 3:
			return (addr >= 0x2000);
		default: {
			throw MapperNotImplementedException();
			break;
		}
	}
}
uint16_t GamePak::GetVRAMAddr() {
	switch (mapperNum)
	{
		case 0:
		case 2:
		case 3:
			return 0x2000;
		default:
			throw MapperNotImplementedException();
			break;
	}
}
uint8_t GamePak::PPURead(uint16_t addr) {
	switch (mapperNum)
	{
		case 0:
		case 2:
		case 3:
			return CHRROM[addr];
		default: {
			throw MemoryAddressNotValidException();
			return -1;
		}
	}
}
void GamePak::PPUWrite(uint16_t addr, uint8_t data) {
	switch (mapperNum)
	{
		case 0:
		case 2:
		case 3:
			CHRROM[addr] = data;
			break;
		default: {
			throw MemoryAddressNotValidException();
			break;
		}
	}
}


GamePak::~GamePak() {
	if (size != -1) delete[] cartridge;
}
