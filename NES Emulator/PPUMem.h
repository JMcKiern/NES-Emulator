#ifndef _PPUMEM_H
#define _PPUMEM_H

#include <cstdint>
#include "CPUMem.h"

class PPUMem {
private:
	/*union {
		uint8_t mem[0x10000];
		struct {
			uint8_t PatternTables[0x2000];
			uint8_t NameTables[0x1F00];
			uint8_t Palettes[0x2100];
			uint8_t Mirrors[0x6000];
		};
	};*/
	CPUMem* cpumem;
protected:
	void Write(uint8_t data, uint8_t* absLocation);
	uint8_t Read(uint8_t* absLocation);
};

#endif // !_PPUMEM_H

