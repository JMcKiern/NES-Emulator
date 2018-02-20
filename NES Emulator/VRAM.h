#pragma once

#include <cstdint>
#include "MirrorManager.h"

class VRAM {
private:
	union {
		uint8_t mem[0x10000];
		struct {
			uint8_t PatternTables[0x2000];
			uint8_t NameTables[0x1F00];
			uint8_t Palettes[0x2100];
			uint8_t Mirrors[0x6000];
		};
	};
	MirrorManager mm;
	void SetupMirrors();
public:
	void Write(uint16_t offset, uint8_t data);
	uint8_t Read(uint16_t offset);

	VRAM();
};

