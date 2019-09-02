#pragma once

#include <cstdint>
#include <memory>
#include "CPU_NES.h"
#include "Mapper.h"
#include "PeripheralConnection.h"
#include "Palette.h"
#include "Display/GLScene2D.h"

class CPU_NES;

class PPU {
private:
	// PPU Memory
	uint8_t vram[0x4000];
	uint8_t PaletteRAM[0x20];
	uint16_t UnMirror(uint16_t offset);
	void Write(uint16_t offset, uint8_t data);
	uint8_t Read(uint16_t offset);

	// PPUCTRL Variables
	uint16_t nameTableAddr;
	uint16_t patternTableAddrSprite;
	uint16_t patternTableAddrBackground;
	uint8_t addrInc;
	uint8_t pixelWidth = 8, spriteHeight;
	bool VBlankShouldNMI;

	// PPUMASK Variables
	bool isInColourMode = true;
	bool shouldClipBackground = false;
	bool shouldClipSprites = false;
	bool shouldShowBackground = false;
	bool shouldShowSprites = false;
	uint8_t colourEmphasis; // Background Colour in Monochrome Mode or
							// Colour Intensity in Colour Mode

	// PPUSTATUS Variables
	bool isSpriteOverflow = false;
	bool isSprite0Hit = false;
	bool isInVBlank = true; // Stays here for 1 cycle upon initialization
	bool shouldSuppressSettingVBL = false;
	bool shouldSuppressNMI = false;

	// OAMADDR Variables
	uint8_t oamAddr;

	// PPUADDR Variables
	uint16_t VRAMPtr;
	uint8_t PPUADDRUpper;

	// PPUDATA Variables
	uint8_t ppudataReadBuffer;

	// PPU Registers - Used to communicate with CPU
	uint8_t openBus = 0;
	long frameCount = 0;
	long openBusLastRefresh = 0;
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
	static const int dispHeight = 240;
	static const int dispWidth = 256;
	uint8_t disp[dispWidth][dispHeight]; // Used for hashing
	bool isOddFrame = false;
	int cycle = 0;		// across 0 <= x <= 340 (x = cycle)
	int scanline = -1;	// down -1 <= y <= 260  (y = scanline)
	void RenderTick();
	void ChoosePixel();
	void RenderPixel(uint8_t outPxl, uint8_t outAttr, bool isSprite);

	// Rendering - Background
	//		Note: this implementation shifts left and takes high bit
	//			whereas NES shifts right and takes low bit
	uint16_t bsr16Bg[2]; // bitmap
	uint8_t bsr8Bg[2]; // palette attributes
	uint8_t attrLatch = 0;
	uint8_t nextNTByte = 0;
	uint8_t nextTileLow = 0;
	uint8_t nextTileHigh = 0;
	uint8_t nextAttrByte = 0;
	uint8_t bgNTTemp = 0;
	void LoadBGTile(int stepNum);
	void FlushBGShifters();
	void ShiftBGShifters();
	//		Scrolling registers
	uint16_t v = 0, t = 0;
	uint8_t x, w;
	void ScrlCoarseXInc();
	void ScrlYInc();


	// Rendering - Sprites
	//	OAM (also known as Sprite Ram or SPR-RAM)
	uint8_t OAM[0x100]; // 64 sprites with 4 bytes each
	uint8_t OAMSL[0x20]; // Up to 8 sprites that are on current scanline
	//	Other
	void SpriteEvaluation();
	uint8_t oamSLAddr;
	uint8_t spriteEvalTemp;
	uint8_t bsrSpr[8][2]; // bitmap data, 0 = low, 1 = high
	uint8_t lchSpr[8]; // attributes
	uint8_t ctrSpr[8]; // x positions
	uint8_t spritePixelsLeft[8];
	int numSpritesOAMSL = 0;
	bool allSpritesEvaluated = false;
	bool hadFirstEmptySprite = false;
	void LoadSpritesForScanline();
	uint8_t ReverseByte(uint8_t byte);
	void ShiftSprShifters();

	void UpdateA12(uint16_t addr);

	GLScene2D* gls;

	Palette palette;

	Mapper** mapperPtrPtr;

public:
	// Frame Check
	bool IsOddFrame();

	// PPU Registers Access Functions for the CPU
	void WriteReg(uint16_t offset, uint8_t data);
	uint8_t ReadReg(uint16_t offset);

	std::string GetDispHash();

	void Tick();
	void PowerUp();

	PPU(CPU_NES* _CPUPtr, Mapper** mapperPtrPtr, GLScene2D* _gls);
};
