#include <iomanip>
#include "CPU_NES.h"

uint16_t CPU_NES::UnMirror(uint16_t offset) {
	if (0x800 <= offset && offset < 0x2000) {
		offset = offset % 0x800;
	}
	else if (0x2008 <= offset && offset < 0x4000) {
		offset = (offset % 0x8) + 0x2000;
	}
	return offset;
}

// CPU Memory
uint8_t CPU_NES::Read(uint16_t offset, bool shouldTick/*= true*/) {
	uint8_t value;
	if (shouldTick) Tick();
	offset = UnMirror(offset);
	if (offset >= 0x0 && offset < 0x800) {
		value = *reinterpret_cast<uint8_t*>(RAM + offset);
	}
	else if ((offset >= 0x2000 && offset <= 0x2007) || (offset == 0x4014)) {
		value = PPUPtr->ReadReg(offset);
	}
	else if (offset >= 0x4020 && offset <= 0xFFFF) {
		value = gamePak->Read(offset);
	}
	else if (offset >= 0x4000 && offset <= 0x4013 || offset == 0x4015) {
		// Audio
		value = 0; // TODO
	}
	else if (offset >= 0x4016 && offset <= 0x4017) {
		// Controllers
		if (offset == 0x4016) {
			value = controller->Read();
		}
		// TODO: Add other controllers
		else {
			value = controller->Read();
			//value = 0;
		}
		value |= (openBus & 0xe0);
	}
	else {
		throw MemoryAddressNotValidException();
	}
	openBus = value;
	return value;
}

void CPU_NES::Write(uint16_t offset, uint8_t data, bool shouldTick/*= true*/) {
	if (shouldTick) Tick();
	offset = UnMirror(offset);
	if (offset >= 0x0 && offset < 0x800) {
		*(RAM + offset) = data;
	}
	else if ((offset >= 0x2000 && offset <= 0x2007) || (offset == 0x4014)) {
		PPUPtr->WriteReg(offset, data);
	}
	else if (offset >= 0x4020 && offset <= 0xFFFF) {
		gamePak->Write(offset, data);
	}
	else if (offset >= 0x4000 && offset <= 0x4013 || offset == 0x4015) {
		// Audio
	}
	else if (offset >= 0x4016) {
		// Controllers
		controller->Write(data);
	}
	else {
		throw MemoryAddressNotValidException();
	}
}

// Logging
void CPU_NES::PrintDebugInfoMesen() {
	PrintDebugInfoMesenBasic();
	(*log) << std::hex << std::uppercase << std::setfill('0');
	//(*log) << " CYC:" << std::setw(3) << (unsigned int)PPUPtr->GetPPUCycle();
	//(*log) << " SL:" << std::setw(2) << (unsigned int)PPUPtr->GetPPUScanline();
	(*log) << '\n';
}

// Other Processors
void CPU_NES::PPURequestingWrite(uint16_t offset, uint8_t data) {
	WriteNoTick(offset, data);
}
uint8_t CPU_NES::PPURequestingRead(uint16_t offset) {
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
void CPU_NES::PowerUp() {
	SetP(0x34);
	SetA(0);
	SetX(0);
	SetY(0);
	SetSP(0xFD);
	SetPC(Read(0xFFFC) + (Read(0xFFFD) << 8)); // RESET VECTOR
}
void CPU_NES::Reset() {
	SetI();
	SetSP(GetSP() - 3);
	SetPC(Read(0xFFFC) + (Read(0xFFFD) << 8)); // RESET VECTOR
}

void CPU_NES::Tick() {
	CPU_6502::Tick();

	for (int i=0;i<3;i++) {
		PPUPtr->Tick();
	}
}

// Constructor
CPU_NES::CPU_NES(Log* _log, PPU* _PPUPtr, GamePak* _gp, Controller* _controller) :
	CPU_6502(_log, 0x800)
{
	PPUPtr = _PPUPtr;
	controller = _controller;
	gamePak = _gp;
}