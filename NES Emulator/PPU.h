#pragma once

#include <cstdint>
#include "CPUMem.h"
#include "VRAM.h"
// TODO:
//		Add Tick()

class PPU {
private:
	// SPR-RAM (Sprite RAM)
	uint8_t SPRRAM[0x100];

	// VRAM (PPU Memory)
	VRAM vram;
	void Write(uint16_t offset, uint8_t data);
	uint8_t Read(uint16_t offset);
	uint8_t ReadNoTick(uint16_t offset);

	CPUMem* CPUMemPtr;
	void UpdateRegisters();
	uint8_t ReadBothControlRegisters();
	uint8_t AccessPPUControlRegister(bool shouldAccessCR1);
	void SetPPUStatusRegister(uint8_t data);

	
	// Control Register 1 Variables	
	uint16_t NN; // NameTable
	uint8_t I;  // Address Increment
	uint8_t S;  // Pattern Table for Sprites
	uint8_t B;  // Pattern Table for Background
	uint8_t H;  // Sprite Height
	uint8_t P;  // PPU Master/Slave
	uint8_t V;  // NMI Enable
	
	// Control Register 2 Variables	
	uint8_t G;   // Greyscale
	uint8_t m;   // Background Left Column Enable
	uint8_t M;   // Sprite Left Column Enable
	uint8_t b;   // Background Enable
	uint8_t s;   // Sprite Enable
	uint8_t BGR; // Colour Emphasis

	// Control Register 1 Variables	
	uint16_t nameTableAddr, patternTableAddrSprite, 
		patternTableAddrBackground;
	uint8_t addrInc;
	uint8_t pixelWidth = 8, pixelHeight;
	bool VBlankShouldNMI;

	// Control Register 2 Variables
	bool isInColourMode, shouldClipBackground, shouldClipSprites,
		shouldShowBackground, shouldShowSprites;
	uint8_t colourEmphasis; //	Background Colour in Monochrome Mode or 
					 // Colour Intensity in Colour Mode

	uint8_t fineX, fineY;
	bool isNextScrollX = true;

	uint16_t VRAMPtr;

public:
	void RunCycle();
	void DMA();
	void SPRRAMTransfer();
	void DATA();
	void SCROLL();
	void ADDR();
	void Render();

	PPU(CPUMem* _CPUMemPtr);
};

