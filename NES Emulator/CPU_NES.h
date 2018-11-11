#pragma once
#include "CPU_6502.h"
#include "PPU.h"
#include "GamePak.h"
#include "Controller.h"

class PPU;

class CPU_NES : public CPU_6502 {
private:
	// Memory
	uint16_t UnMirror(uint16_t offset);
	uint8_t Read(uint16_t offset, bool shouldTick = true);
	void Write(uint16_t offset, uint8_t data, bool shouldTick = true);
	uint8_t openBus;

	// Logging
	void PrintDebugInfoMesen();

	// Other Processors
	PPU* PPUPtr;
	Controller* controller;
	GamePak* gamePak;

	// Timing
	void Tick();

	// Start Up
	void StartCycle();
	void Reset();

public:
	// Other Processors
	void PPURequestingWrite(uint16_t data, uint8_t offset);
	uint8_t PPURequestingRead(uint16_t offset);

	// Start Up
	void PowerUp();

	CPU_NES(Log* _log, PPU* _PPUPtr, GamePak* _gp, Controller* _controller);
};