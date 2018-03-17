#include "CPU_NES.h"

void CPU_NES::SetupMirrors() {
	mm.AddMirror(0x0, { 0x800, 0x1000, 0x1800 }, 0x800);

	std::vector<uint16_t> v2;
	for (int i = 0x8; i < 0x3fff; i += 0x8) {
		v2.push_back(0x2000 + i);
	}
	mm.AddMirror(0x2000, v2, 0x8);
}

// CPU Memory
uint8_t CPU_NES::Read(uint16_t offset, bool shouldTick/*= true*/) { 
	if (shouldTick) Tick();
	offset = mm.UnMirrorAddr(offset);
	if (offset >= 0x0 && offset < 0x800) {
		return *reinterpret_cast<uint8_t*>(RAM + offset);
	}
	else if ((offset >= 0x2000 && offset < 0x2007) || (offset == 0x4014)) {
		return PPUPtr->ReadReg(offset);
	}
	else if (offset >= 0x4200 && offset <= 0xFFFF) {
		return gamePak->Read(offset);
	}
	else {
		throw std::out_of_range::out_of_range("CPU Read offset out of range");
	}
}

void CPU_NES::Write(uint16_t offset, uint8_t data, bool shouldTick/*= true*/) {
	if (shouldTick) Tick();
	offset = mm.UnMirrorAddr(offset);
	if (offset >= 0x0 && offset < 0x800) {
		*(RAM + offset) = data;
	}
	else if ((offset >= 0x2000 && offset < 0x2007) || (offset == 0x4014)) {
		PPUPtr->WriteReg(offset, data);
	}
	else if (offset >= 0x4200 && offset <= 0xFFFF) {
		gamePak->Write(offset, data);
	}
	else {
		throw std::out_of_range::out_of_range("CPU Write offset out of range");
	}
}
// Other Processors
void CPU_NES::PPURequestingWrite(uint8_t offset, uint8_t data) {
	WriteNoTick(offset, data);
}
uint8_t CPU_NES::PPURequestingRead(uint8_t offset) {
	return ReadNoTick(offset);
}
void CPU_NES::StartCycle() {
	Tick();
	Tick();
	Tick();
	Tick();
	Tick();
	SetPC(Read(0xFFFC) + (Read(0xFFFD) << 8)); // RESET VECTOR
	SetI();
}
void CPU_6502::PowerUp() {
	SetP(0x34, true);
	A = 0;
	X = 0;
	Y = 0;
	PC = Read(0xFFFC) + (Read(0xFFFD) << 8); // RESET VECTOR
	SP = 0xFD;
}
void CPU_NES::StartNESTEST() {
	SetP(0x24, false);
	SetSP(0xfd);
}
// Constructor
CPU_NES::CPU_NES(Log* _log, GamePak* _gp) :
	CPU_6502(_log, 0x800)
{
	gamePak = _gp;
	SetupMirrors();
}
CPU_NES::~CPU_NES() {
	delete RAM;
}
