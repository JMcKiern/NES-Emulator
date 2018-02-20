#include "PPU.h"
#include "Colour.h"

// CPU Memory
void PPU::Write(uint16_t offset, uint8_t data) {
	vram.Write(offset, data);
}
uint8_t PPU::Read(uint16_t offset) {
	uint8_t data = vram.Read(offset);
	return data;
}
uint8_t PPU::ReadNoTick(uint16_t offset) {
	uint8_t data = vram.Read(offset);
	return data;
}
void PPU::UpdateRegisters() {
	uint8_t CR1 = AccessPPUControlRegister(true);
	uint8_t CR2 = AccessPPUControlRegister(false);
	
	// CR1
	NN = (CR1 & 0x11);
	I = ((CR1 >> 2) & 0x1);
	S = ((CR1 >> 3) & 0x1);
	B = ((CR1 >> 4) & 0x1);
	H = ((CR1 >> 5) & 0x1);
	P = ((CR1 >> 6) & 0x1); // NESDOC says not used but website says master/slave
	V = ((CR1 >> 7) & 0x1);

	// CR2
	G = !(CR2 & 0x1);
	m = !((CR2 >> 1) & 0x1);
	M = !((CR2 >> 2) & 0x1);
	b = ((CR2 >> 3) & 0x1);
	s = ((CR2 >> 4) & 0x1);
	BGR = ((CR2 >> 5) & 0x111);
}
//uint8_t PPU::ReadBothControlRegisters() {
//	uint8_t CR1 = AccessPPUControlRegister(true);
//	uint8_t CR2 = AccessPPUControlRegister(false);
//
//	{
//		uint8_t nameTable = (CR1 & 0x11);
//		if (nameTable == 0) nameTableAddr = 0x2000;
//		else if (nameTable == 1) nameTableAddr = 0x2400;
//		else if (nameTable == 2) nameTableAddr = 0x2800;
//		else if (nameTable == 3) nameTableAddr = 0x2C00;
//
//		addrInc = ((CR1 >> 2) & 0x1) ? 32 : 1;
//		patternTableAddrSprite = ((CR1 >> 3) & 0x1) ? 0x1000 : 0x0;
//		patternTableAddrBackground = ((CR1 >> 4) & 0x1) ? 0x1000 : 0x0;
//		pixelHeight = ((CR1 >> 5) & 0x1) ? 16 : 8;
//		// Bit 6 not used
//		VBlankShouldNMI = ((CR1 >> 7) & 0x1);
//	}
//	{
//		isInColourMode = !(CR2 & 0x1);
//		shouldClipBackground = !((CR2 >> 1) & 0x1);
//		shouldClipSprites = !((CR2 >> 2) & 0x1);
//		shouldShowBackground = ((CR2 >> 3) & 0x1);
//		shouldShowSprites = ((CR2 >> 4) & 0x1);
//		colour = ((CR2 >> 5) & 0x111);
//	}
//}
uint8_t PPU::AccessPPUControlRegister(bool shouldAccessCR1) {
	if (shouldAccessCR1) {
		return CPUMemPtr->Read(0x2000);
	}
	else {
		return CPUMemPtr->Read(0x2001);
	}
}
void PPU::SetPPUStatusRegister(uint8_t data) {
	CPUMemPtr->Write(0x2002, data);
}

void PPU::RunCycle() {
}
void PPU::DMA() {
	uint8_t operand = Read(0x4014);
	for (int i = 0; i < 0x100; i++) {
		SPRRAM[i] = Read((operand * 0x100) + i);
	}
}

PPU::PPU(CPUMem* _CPUMemPtr) {
	CPUMemPtr = _CPUMemPtr;
}