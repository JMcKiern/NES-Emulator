#pragma once

#include <cstdint>
#include <fstream>

class Mapper {
protected:
	uint8_t * PRGROMU;
	uint8_t* PRGROML;
	uint8_t* PRGRAM;
	uint8_t* CHRROM;
	uint8_t* CHRRAM;
	uint8_t SRAM[0x8000 - 0x6000];
	uint8_t ExpansionROM[0x6000 - 0x4200];

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

	uint32_t GetOffsetPRG(uint8_t bankNum);
	uint32_t GetOffsetCHR(uint8_t bankNum);
	uint8_t* GetPtrPRG(uint8_t bankNum);
	uint8_t* GetPtrCHR(uint8_t bankNum);

public:
	virtual uint8_t Read(uint16_t addr);
	virtual void Write(uint16_t addr, uint8_t data);

	// TODO: Not sure about these 2
	// Does VRAM always start at 0x2000?
	bool isVRAM(uint16_t addr);
	uint16_t GetVRAMAddr();

	virtual uint8_t PPURead(uint16_t addr);
	virtual void PPUWrite(uint16_t addr, uint8_t data);
	bool UsingVerticalMirroring();

	Mapper(std::ifstream& f);
	~Mapper();
};
