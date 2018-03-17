#include "GamePak.h"
#include <iostream>

void GamePak::LoadIntoMem(uint16_t gpOffset, uint16_t len, uint16_t cartridgeOffset) {
}
uint8_t GamePak::Read(uint16_t addr) {
	if (0x8000 <= addr && addr < 0xC000) {
		return PRGROML[addr - 0x8000];
	}
	if (0xC000 <= addr && addr < 0x10000) {
		return PRGROMU[addr - 0xC000];
	}
	return 0;
}
void GamePak::Write(uint16_t addr, uint8_t data) {
	RegisterUpdate(addr, data);
}
uint16_t GamePak::GetOffsetPRG(uint8_t bankNum) {
	if (bankNum < 0 || bankNum > num16kPRGBanks) {
		std::out_of_range::out_of_range("Attempted to access nonexistant bank number.");
	}
	uint16_t offset = 0x10; // header
	if (isTrainer) offset += 0x200;
	offset += bankNum * 0x4000;
	return offset;
}
uint16_t GamePak::GetOffsetCHR(uint8_t bankNum) {
	if (bankNum < 0 || bankNum > num16kPRGBanks) {
		std::out_of_range::out_of_range("Attempted to access nonexistant bank number.");
	}
	uint16_t offset = 0x10; // header
	if (isTrainer) offset += 0x200;
	offset += bankNum * 0x4000;
	return offset;
}
void GamePak::RegisterUpdate(uint16_t addr, uint8_t data) {
	switch (mapperNum) {
		case 0: break;
		default: {
			throw MapperNotImplementedException();
			break;
		}
	}
}
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
			uint16_t offset = GetOffsetPRG(0);
			uint8_t* ptr = reinterpret_cast<uint8_t*>(cartridge + offset);
			PRGROML = PRGROMU = ptr;
			break;
		}
		default: {
			throw MapperNotImplementedException();
			break;
		}
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

GamePak::~GamePak() {
	if (size != -1) delete[] cartridge;
}
