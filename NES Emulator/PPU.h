#pragma once

#include <cstdint>
#include "PeripheralConnection.h"
#include "VRAM.h"
#include "CPU_NES.h"
// TODO:
//		Add Tick()

class CPU_NES;

class PPU {
private:

	// VRAM (PPU Memory)
	VRAM vram;
	void Write(uint16_t offset, uint8_t data);
	uint8_t Read(uint16_t offset);
	uint8_t ReadNoTick(uint16_t offset);

	// PPUCTRL Variables	
	uint16_t nameTableAddr;
	uint16_t patternTableAddrSprite;
	uint16_t patternTableAddrBackground;
	uint8_t addrInc;
	uint8_t pixelWidth = 8, spriteHeight;
	bool VBlankShouldNMI;

	// PPUMASK Variables
	bool isInColourMode;
	bool shouldClipBackground;
	bool shouldClipSprites;
	bool shouldShowBackground;
	bool shouldShowSprites;
	uint8_t colourEmphasis; // Background Colour in Monochrome Mode or 
							// Colour Intensity in Colour Mode

	// PPUSTATUS Variables
	bool isSpriteOverflow;
	bool isSprite0Hit;
	bool isInVBlank;
	uint8_t lastWrite = 0;

	// PPUSCROLL Variables
	uint8_t fineX, fineY;
	bool isNextScrollX = true;

	// OAMADDR Variables
	uint8_t oamAddr;

	// PPUADDR Variables
	uint16_t VRAMPtr;
	bool isNextByteUpper = true;
	uint8_t PPUADDRUpper;

	// PPU Registers - Used to communicate with CPU
	void PPUCTRL(uint8_t data);
	void PPUMASK(uint8_t data);
	uint8_t PPUSTATUS();
	void OAMADDR(uint8_t data);
	void OAMDATA(uint8_t data); uint8_t OAMDATA();
	void PPUSCROLL(uint8_t data);
	void PPUADDR(uint8_t data);
	void PPUDATA(uint8_t data);	uint8_t PPUDATA();
	void OAMDMA(uint8_t data);
	PeripheralConnection cpuNMIConnection;
	CPU_NES* CPUPtr;

	// Rendering
	int dispHeight = 256;
	int dispWidth = 240;
	int disp[256][240];
	bool isOddFrame = false;
	int cycle = 0;		// going across 0 <= x <= 340
	int scanline = -1;	// going down -1 <= y <= 260
	void RenderTick();
	void RenderPixel();

	uint16_t bsr16Bg[2];
	uint8_t bsr8Bg[2];

	// Rendering - Sprites
	//	OAM (also known as Sprite Ram or SPR-RAM)
	uint8_t OAM[0x100]; // 64 sprites with 4 bytes each
	uint8_t OAMSL[0x20]; // sprites that are on current scanline 8 sprites with 4 bytes each
	//	Other
	void SpriteEvaluation();
	uint8_t oamSLAddr;
	uint8_t spriteEvalTemp;
	uint8_t bsrSpr[8][2];
	uint8_t lchSpr[8];
	uint8_t ctrSpr[8];


public:
	void RunCycle();

	// PPU Registers Access Functions for the CPU
	void WriteReg(uint16_t offset, uint8_t data);
	uint8_t ReadReg(uint16_t offset);

	void Tick();
	void PowerUp();

	PPU(CPU_NES* _CPUPtr);
};