#pragma once
#include "CPU_6502.h"
#include "MirrorManager.h"
#include "GamePak.h"
#include "PPU.h"

class CPU_NES : public CPU_6502 {
	// Memory
	MirrorManager mm;
	void SetupMirrors();
	uint8_t Read(uint16_t offset, bool shouldTick = true);
	void Write(uint16_t offset, uint8_t data, bool shouldTick = true);

	// Other Processors
	PPU* PPUPtr;

	GamePak* gamePak;

	void StartCycle();

public:
	void PPURequestingWrite(uint8_t data, uint8_t offset);
	uint8_t PPURequestingRead(uint8_t offset);

	void StartNESTEST();

	CPU_NES(Log* _log, GamePak* _gp);
	~CPU_NES();
};