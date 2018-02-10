#include <cstdint>
#include "PPUMem.h"

void PPUMem::Write(uint8_t data, uint8_t* absLocation) {
	uintptr_t relLocation = reinterpret_cast<uintptr_t>(absLocation - *mem);
	if (relLocation >= 0x0 && relLocation <= 0x3fff) {
		*absLocation = data;
		*(absLocation + 0x4000) = data;
	}
	else if (relLocation >= 0x2000 && relLocation <= 0x2007) {
		for (int i = 0; i < 0x3fff; i += 0x8) {
			*(absLocation + i) = data;
		}
	}
	else {
		*absLocation = data;
	}
}
uint8_t PPUMem::Read(uint8_t* absLocation) {
	return *absLocation;
}