#pragma once
#include "CPU_6502.h"
#include "PPU.h"
#include "GamePak.h"
#include "Controller.h"

class CPU_NES : public CPU_6502 {
	// Memory
	uint16_t UnMirror(uint16_t offset);
	uint8_t Read(uint16_t offset, bool shouldTick = true);
	void Write(uint16_t offset, uint8_t data, bool shouldTick = true);

	// Logging
	void PrintDebugInfoMesen();

	// Other Processors
	PPU* PPUPtr;

	Controller* controller;
	GamePak* gamePak;

	void StartCycle();
	void Reset();
	void Tick();

public:
	void PPURequestingWrite(uint16_t data, uint8_t offset);
	uint8_t PPURequestingRead(uint16_t offset);

	void PowerUp();

	CPU_NES(Log* _log, PPU* _PPUPtr, GamePak* _gp, Controller* _controller);
};