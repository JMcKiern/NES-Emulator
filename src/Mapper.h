#pragma once

#include <cstdint>
#include <fstream>

enum {
	ONE_SCREEN,
	ONE_SCREEN_LOWER_BANK,
	ONE_SCREEN_UPPER_BANK,
	VERTICAL,
	HORIZONTAL,
	FOUR_SCREEN
};

class Mapper {
protected:
	uint8_t * PRGROMU;
	uint8_t* PRGROML;
	uint8_t* PRGRAM = nullptr;
	uint8_t* CHRROM;
	uint8_t* CHRRAM = nullptr;

	char* cartridge;
	int size = -1;

	uint8_t num16kPRGBanks;
	uint8_t num8kCHRBanks;
	uint8_t num8kRAMBanks;
	bool usingVerticalMirroring;
	bool isBatteryBackedRAM;
	bool isTrainer;
	bool usingFourScreenMirroring;
	int mapperNum;

	void LoadINES(std::ifstream& f);
	void SetupINES();

	uint8_t* GetPtrPRG(uint8_t bankNum);
	uint8_t* GetPtrCHR(uint8_t bankNum);
	uint32_t GetOffsetPRG(uint8_t bankNum, uint16_t bankSize);
	uint32_t GetOffsetCHR(uint8_t bankNum, uint16_t bankSize);
	uint8_t* GetPtrPRG(uint8_t bankNum, uint16_t bankSize);
	uint8_t* GetPtrCHR(uint8_t bankNum, uint16_t bankSize);

	int GetNameTableNum(uint16_t addr);
	int mirroringMode;

public:
	virtual uint8_t Read(uint16_t addr);
	virtual void Write(uint16_t addr, uint8_t data);
	uint16_t UnMirror(uint16_t addr);

	// TODO: Not sure about these 2
	// Does VRAM always start at 0x2000?
	bool isVRAM(uint16_t addr);
	uint16_t GetVRAMAddr();

	virtual uint8_t PPURead(uint16_t addr);
	virtual void PPUWrite(uint16_t addr, uint8_t data);

	Mapper(std::ifstream& f);
	~Mapper();
};
