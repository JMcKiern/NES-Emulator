#include <stdexcept>
#include "CPUMem.h"

void CPUMem::Write(uint16_t offset, uint8_t data) {
	uint8_t* absLocation = reinterpret_cast<uint8_t*>(mem + offset);
	/*if (offset >= 0x0 && offset <= 0x7ff) {
		*absLocation = data;
		*(absLocation + 0x800) = data;
		*(absLocation + 0x1000) = data;
		*(absLocation + 0x1800) = data;
	}
	else if (offset >= 0x2000 && offset <= 0x2007) {
		for (int i = 0; i < 0x3fff; i += 0x8) {
			*(absLocation + i) = data;
		}
	}
	else {*/
		*absLocation = data;
	//}
}
uint8_t CPUMem::Read(uint16_t offset) {
	return *reinterpret_cast<uint8_t*>(mem + offset);
}

#ifdef _DEBUG_MODE
CPUMem::CPUMem() {
	std::cout << "Mem Addres: " << std::hex << reinterpret_cast<uintptr_t>(mem) << '\n';
}
#endif