#include "Mapper.h"

uint8_t* Mapper::GetPtrPRG(uint8_t bankNum) {
	uint32_t offset = GetOffsetPRG(bankNum, 0x4000);
	return reinterpret_cast<uint8_t*>(cartridge + offset);
}
uint8_t* Mapper::GetPtrCHR(uint8_t bankNum) {
	uint32_t offset = GetOffsetCHR(bankNum, 0x2000);
	return reinterpret_cast<uint8_t*>(cartridge + offset);
}
uint32_t Mapper::GetOffsetPRG(uint8_t bankNum, uint16_t bankSize) {
	if (bankNum * bankSize >= num16kPRGBanks * 0x4000)
		throw std::out_of_range("Attempted to access nonexistant bank number.");
	uint32_t offset = 0x10; // Header
	if (isTrainer) offset += 0x200; // Trainer
	offset += bankSize * bankNum; // PRG Banks
	if (offset + bankSize > size)
		throw std::out_of_range("Out of range cartidge access.");
	return offset;
}
uint32_t Mapper::GetOffsetCHR(uint8_t bankNum, uint16_t bankSize) {
	if (bankNum * bankSize >= num8kCHRBanks * 0x2000)
		throw std::out_of_range("Attempted to access nonexistant bank number.");
	uint32_t offset = 0x10; // Header
	if (isTrainer) offset += 0x200; // Trainer
	offset += 0x4000 * num16kPRGBanks; // PRG Banks
	offset += bankSize * bankNum; // CHR Banks
	if (offset + bankSize > size)
		throw std::out_of_range("Out of range cartidge access.");
	return offset;
}
uint8_t* Mapper::GetPtrPRG(uint8_t bankNum, uint16_t bankSize) {
	uint32_t offset = GetOffsetPRG(bankNum, bankSize);
	return reinterpret_cast<uint8_t*>(cartridge + offset);
}
uint8_t* Mapper::GetPtrCHR(uint8_t bankNum, uint16_t bankSize) {
	uint32_t offset = GetOffsetCHR(bankNum, bankSize);
	return reinterpret_cast<uint8_t*>(cartridge + offset);
}
bool Mapper::isVRAM(uint16_t addr) {
	return (addr >= 0x2000);
}
uint16_t Mapper::GetVRAMAddr() {
	return 0x2000;
}

void Mapper::LoadINES(std::ifstream& f) {
	f.seekg(0, std::ios_base::end);
	std::streampos sizef = f.tellg();
	size = static_cast<int>(sizef);
	cartridge = new char[size];

	f.seekg(0, std::ios::beg);
	f.read(cartridge, sizef);
}
void Mapper::SetupINES() {
	num16kPRGBanks = cartridge[4];
	num8kCHRBanks = cartridge[5];
	usingVerticalMirroring = (cartridge[6] >> 0) & 0x1;
	mirroringMode = usingVerticalMirroring ? VERTICAL : HORIZONTAL;
	isBatteryBackedRAM = (cartridge[6] >> 1) & 0x1;
	isTrainer = (cartridge[6] >> 2) & 0x1;
	usingFourScreenMirroring = (cartridge[6] >> 3) & 0x1;
	mirroringMode = usingFourScreenMirroring ? FOUR_SCREEN : mirroringMode;
	mapperNum = ((cartridge[6] >> 4) & 0xF) + (cartridge[7] & 0xF0);
	num8kRAMBanks = cartridge[8];
}
int Mapper::GetNameTableNum(uint16_t offset) {
	if (0x2400 <= offset && offset <= 0x27FF) { // NT 1
		return 1;
	}
	else if (0x2800 <= offset && offset <= 0x2BFF) { // NT 2
		return 2;
	}
	else if (0x2C00 <= offset && offset <= 0x2FFF) { // NT 3
		return 3;
	}
	throw std::runtime_error("Not a valid nametable address!");
}
uint16_t Mapper::UnMirror(uint16_t offset) {
	if (0x2400 <= offset && offset <= 0x2FFF) { // Nametable 1, 2 or 3
		int nameTableNum = GetNameTableNum(offset);
		if (mirroringMode == HORIZONTAL) {
			if (nameTableNum == 1 || nameTableNum == 3) 
				offset -= 0x400;
		}
		else if (mirroringMode == VERTICAL) {
			if (nameTableNum == 2 || nameTableNum == 3)
				offset -= 0x800;
		}
		else if (mirroringMode == ONE_SCREEN) {
			if (nameTableNum == 1 || nameTableNum == 3) 
				offset -= 0x400;
			if (nameTableNum == 2 || nameTableNum == 3)
				offset -= 0x800;
		}
		else if (mirroringMode == FOUR_SCREEN) {
			// Do nothing
		}
	}
	return offset;
}
uint8_t Mapper::Read(uint16_t addr) {
	if (0x6000 <= addr && addr < 0x8000) {
		if (PRGRAM != NULL)
			return PRGRAM[addr - 0x6000];
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

void Mapper::Write(uint16_t addr, uint8_t data) {
	// Do nothing
	if (0x6000 <= addr && addr < 0x8000) {
		if (PRGRAM != NULL)
			PRGRAM[addr - 0x6000] = data;
	}
}

uint8_t Mapper::PPURead(uint16_t addr) {
	if (num8kCHRBanks > 0) {
		return CHRROM[addr];
	}
	else
		return CHRRAM[addr];
}
void Mapper::PPUWrite(uint16_t addr, uint8_t data) {
	if (num8kCHRBanks > 0) {
		CHRROM[addr] = data;
	}
	else
		CHRRAM[addr] = data;
}

void Mapper::IRQClock() {
	// Do nothing
}

uint8_t Mapper::GetMapperNum() {
	return mapperNum;
}

Mapper::Mapper(std::ifstream& f) {
	LoadINES(f);
	SetupINES();
}
Mapper::~Mapper() {
	if (size != -1) delete[] cartridge;
	if (PRGRAM != nullptr) delete[] PRGRAM;
	if (CHRRAM != nullptr) delete[] CHRRAM;
}
