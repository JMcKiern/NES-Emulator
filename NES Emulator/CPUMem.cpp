#include <stdexcept>
#include "Exceptions.h"
#include "CPUMem.h"


void CPUMem::SetupMirrors() {
	mm.AddMirror(0x0, { 0x800, 0x1000, 0x1800 }, 0x800);

	std::vector<uint16_t> v2;
	for (int i = 0x8; i < 0x3fff; i += 0x8) {
		v2.push_back(0x2000 + i);
	}
	mm.AddMirror(0x2000, v2, 0x8);
}

void CPUMem::Write(uint16_t offset, uint8_t data) {
	uint16_t baseOffset = mm.UnMirrorAddr(offset);
	*(mem + baseOffset) = data;
}
uint8_t CPUMem::Read(uint16_t offset) {
	uint16_t baseOffset = mm.UnMirrorAddr(offset);
	return *reinterpret_cast<uint8_t*>(mem + baseOffset);
}

CPUMem::CPUMem() {
	SetupMirrors();
#ifdef _DEBUG_MODE
	std::cout << "Mem Address: " << std::hex << reinterpret_cast<uintptr_t>(mem) << '\n';
#endif
}
CPUMem::CPUMem(bool shouldSetupBlank) {
	if (!shouldSetupBlank) SetupMirrors();
#ifdef _DEBUG_MODE
	std::cout << "Mem Address: " << std::hex << reinterpret_cast<uintptr_t>(mem) << '\n';
#endif
}