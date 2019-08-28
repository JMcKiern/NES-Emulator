#pragma once
#include "CPU_6502.h"
#include "PPU.h"
#include "APU.h"
#include "Mapper.h"
#include "IO.h"

class PPU;
class APU;

class CPU_NES : public CPU_6502 {
private:
	// Memory
	uint16_t UnMirror(uint16_t offset);
	uint8_t Read(uint16_t offset, bool shouldTick = true);
	void Write(uint16_t offset, uint8_t data, bool shouldTick = true);

	// Logging
	void PrintDebugInfoMesen();

	// Other Processors
	PPU* PPUPtr;
	APU* APUPtr;
	IO* IOPtr;
	Mapper** mapperPtrPtr;

	// Timing
	void Tick();

	// Start Up
	void StartCycle();
	void Reset();

public:
	// Other Processors
	void PPURequestingWrite(uint16_t offset, uint8_t data);
	uint8_t PPURequestingRead(uint16_t offset);

	// Start Up
	void PowerUp();

	// Public so MMC3 can return it if PRGRAM disabled
	uint8_t openBus;

	CPU_NES(PPU* _PPUPtr, APU* _APUPtr, Mapper** _mapperPtr, IO* _io);
};