#include <cstdint>
#include <vector>
#include "VRAM.h"

void VRAM::SetupMirrors() {
	mm.AddMirror(0x0, { 0x4000, 0x8000, 0xc000 }, 0x4000);
	mm.AddMirror(0x2000, {0x3000}, 0xf00);

	std::vector<uint16_t> v2;
	for (int i = 0x8; i < 0x3fff; i += 0x8) {
		v2.push_back(0x2000 + i);
	}
	mm.AddMirror(0x0, v2, 0x8);
}

void VRAM::Write(uint16_t offset, uint8_t data) {
	uint16_t baseOffset = mm.UnMirrorAddr(offset);
	*(mem + baseOffset) = data;
}
uint8_t VRAM::Read(uint16_t offset) {
	uint16_t baseOffset = mm.UnMirrorAddr(offset);
	return *reinterpret_cast<uint8_t*>(mem + baseOffset);
}

VRAM::VRAM() {
	SetupMirrors();
#ifdef _DEBUG_MODE
	std::cout << "Mem Address: " << std::hex << reinterpret_cast<uintptr_t>(mem) << '\n';
#endif
}