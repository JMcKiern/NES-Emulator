#pragma once

#include <string>
#include <fstream>
#include "Exceptions.h"


// TODO:
//		Implement Read and Write

class GamePak {
	uint8_t* PRGROMU;
	uint8_t* PRGROML;
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

	void LoadIntoMem(uint16_t gpOffset, uint16_t len, uint16_t memOffset);

	bool CheckINes();
	void SetupINes();
	uint16_t GetOffsetPRG(uint8_t bankNum);
	uint16_t GetOffsetCHR(uint8_t bankNum);
	void RegisterUpdate(uint16_t addr, uint8_t data);
public:
	void LoadINes(std::string filename);
	uint8_t Read(uint16_t addr);
	void Write(uint16_t addr, uint8_t data);

	~GamePak();
};

