#include <exception>
#include "PPU.h"
#include "Colour.h"

// PPU Memory
uint16_t PPU::UnMirror(uint16_t offset) {
	if (0x4000 <= offset && offset < 0x10000) {
		offset = offset % 0x4000;
	}
	if (0x3f00 <= offset && offset < 0x4000) {
		offset = offset % 0x20 + 0x3f00;
		// $3F10/$3F14/$3F18/$3F1C are mirrors of $3F00/$3F04/$3F08/$3F0C
		if ((offset & 0x3) == 0 && offset >= 0x3f10)
			offset -= 0x10;
	}
	else if (0x3000 <= offset && offset < 0x3f00) {
		offset -= 0x1000;
	}
	return offset;
}
// TODO: What about vram ptr
void PPU::Write(uint16_t offset, uint8_t data) {
	offset = UnMirror(offset);
	if (0x3f00 <= offset && offset <= 0x3f20) {
		// Palette Ram
		PaletteRAM[offset - 0x3f00] = data;
	}
	else {
		if (gp->isVRAM(offset)) {
			uint16_t vramOffset = gp->GetVRAMAddr();
			*(vram + (offset - vramOffset)) = data;
		}
		else {
			gp->PPUWrite(offset, data);
		}
	}
}
uint8_t PPU::Read(uint16_t offset) {
	offset = UnMirror(offset);
	if (0x3f00 <= offset && offset <= 0x3f20) {
		// Palette Ram
		if (isInColourMode)
			return PaletteRAM[offset - 0x3f00];
		else
			return (PaletteRAM[offset - 0x3f00] & 0x30);
	}
	else {
		if (gp->isVRAM(offset)) {
			uint16_t vramOffset = gp->GetVRAMAddr();
			return vram[offset - vramOffset];
		}
		else {
			return gp->PPURead(offset);
		}
	}
}

void PPU::WriteReg(uint16_t offset, uint8_t data) {
	regLatch = data;
	switch (offset) {
		case 0x2000: PPUCTRL(data); break;
		case 0x2001: PPUMASK(data); break;
		case 0x2003: OAMADDR(data); break;
		case 0x2004: OAMDATA(data); break;
		case 0x2005: PPUSCROLL(data); break;
		case 0x2006: PPUADDR(data); break;
		case 0x2007: PPUDATA(data); break;
		case 0x4014: OAMDMA(data); break;
		default: break;
	}
}
uint8_t PPU::ReadReg(uint16_t offset) {
	switch (offset) {
		case 0x2002: return PPUSTATUS();
		case 0x2004: return OAMDATA();
		case 0x2007: return PPUDATA();
		default:
			isNextByteUpper = true;
			return regLatch;
	}
}
void PPU::PPUCTRL(uint8_t data) {
	uint8_t nameTable = (data & 0x11);
	if (nameTable == 0) nameTableAddr = 0x2000;
	else if (nameTable == 1) nameTableAddr = 0x2400;
	else if (nameTable == 2) nameTableAddr = 0x2800;
	else if (nameTable == 3) nameTableAddr = 0x2C00;

	addrInc = ((data >> 2) & 0x1) ? 32 : 1;
	patternTableAddrSprite = ((data >> 3) & 0x1) ? 0x1000 : 0x0;
	patternTableAddrBackground = ((data >> 4) & 0x1) ? 0x1000 : 0x0;
	spriteHeight = ((data >> 5) & 0x1) ? 16 : 8;
	// Bit 6 not used (according to NESDoc, nesdev wiki says otherwise)
	// https://wiki.nesdev.com/w/index.php/PPU_programmer_reference#PPUCTRL
	VBlankShouldNMI = ((data >> 7) & 0x1);
}
void PPU::PPUMASK(uint8_t data) {
	isInColourMode = !(data & 0x1);
	shouldClipBackground = !((data >> 1) & 0x1);
	shouldClipSprites = !((data >> 2) & 0x1);
	shouldShowBackground = ((data >> 3) & 0x1);
	shouldShowSprites = ((data >> 4) & 0x1);
	colourEmphasis = ((data >> 5) & 0x111);
}
uint8_t PPU::PPUSTATUS() { // Potential Error: 5 LSB should not be zero
	isNextByteUpper = true; // Reset data bus
	uint8_t status = 0;
	status |= (regLatch & 0x1F);
	status |= ((isSpriteOverflow & 0x1) << 5);
	status |= ((isSprite0Hit & 0x1) << 6);
	if (isInVBlank && !hasNotifiedVBlank) {
		status |= ((isInVBlank & 0x1) << 7);
		hasNotifiedVBlank = true;
	}
	return status;
}
void PPU::OAMADDR(uint8_t data) {
	oamAddr = data;
}
void PPU::OAMDATA(uint8_t data) {
	OAM[oamAddr] = data;
	oamAddr++;
}
uint8_t PPU::OAMDATA() {
	regLatch = OAM[oamAddr];
	return OAM[oamAddr];
}
void PPU::PPUSCROLL(uint8_t data) {
	if (isNextScrollX) {
		fineX = data;
	}
	else {
		fineY = data;
	}
	isNextScrollX = !isNextScrollX;
}
void PPU::PPUADDR(uint8_t data) {
	if (isNextByteUpper) {
		PPUADDRUpper = data;
	}
	else {
		VRAMPtr = (PPUADDRUpper << 8) + data;
	}
	isNextByteUpper = !isNextByteUpper;
}
void PPU::PPUDATA(uint8_t data) {
	Write(VRAMPtr, data);
	VRAMPtr += addrInc;
}
uint8_t PPU::PPUDATA() {
	uint8_t data = Read(VRAMPtr);
	regLatch = data;
	VRAMPtr += addrInc;
	return data;
}
void PPU::OAMDMA(uint8_t data) {
	for (int i = 0; i < 0x100; i++) {
		OAM[(oamAddr + i) % 0x100] = CPUPtr->PPURequestingRead((data * 0x100) + i);
	}
}

// https://forums.nesdev.com/viewtopic.php?f=3&t=13226
// http://wiki.nesdev.com/w/index.php/PPU_sprite_evaluation
void PPU::SpriteEvaluation() {
	static int n = 0;
	static int m = 0;
	if (cycle == 0) {
		n = m = 0;
		oamSLAddr = 0;
		allSpritesEvaluated = false;
		numSpritesOAMSL = 0;
		hadFirstEmptySprite = false;
	}
	else if (1 <= cycle && cycle <= 64) {
		bool isOddCycle = cycle % 2 == 1;
		if (isOddCycle) {
			// It is unclear whether OAMDATA() should return 0xFF
			// (as in Sprite Evaluation page on wiki) or if there
			// should be no OAMDATA() call and just set it to 0xFF
			// In order to make sprite_ram.nes work the 2nd option
			// should be chosen.
			spriteEvalTemp = 0xFF;
		}
		else {
			OAMSL[oamSLAddr] = spriteEvalTemp;
			oamSLAddr = (oamSLAddr + 1) % 0x20;
		}
	}
	else if (65 <= cycle && cycle <= 256) {
		bool isOddCycle = cycle % 2 == 1;
		if (isOddCycle) {
			// Read
			spriteEvalTemp = OAM[n * 4 + m];
		}
		else {
			// Write
			if (!allSpritesEvaluated) {
				if (numSpritesOAMSL < 8) { // Step 2.2b
					OAMSL[oamSLAddr] = spriteEvalTemp;
					if (m == 0) {
						// Step 2.1
						uint8_t spriteY = OAMSL[oamSLAddr - (oamSLAddr % 4)];
						if (spriteY <= scanline && // Should copy sprite
							scanline < (spriteY + spriteHeight)) {
							// Step 2.1a
							m++;
							oamSLAddr = (oamSLAddr + 1) % 0x20;
						}
						else { // Sprite not in range
							n++;
							if (n == 65) {
								n = 0;
								allSpritesEvaluated = true;
							}
							m = 0;
						}
					}
					else {
						m++;
						oamSLAddr = (oamSLAddr + 1) % 0x20;
						if (m == 4) { // Finished copying sprite
							// Step 2.2
							n++;
							if (n == 65) {
								// Step 2.2a
								n = 0;
								allSpritesEvaluated = true;
							}
							m = 0;
							numSpritesOAMSL++;
						}
					}
				}
				else {
					// Step 2.2c
					// Step 2.3
					uint8_t spriteY = OAMSL[oamSLAddr - m];
					if (spriteY <= scanline && // Should copy sprite
						scanline < (spriteY + spriteHeight)) {
						// Step 2.3a
						isSpriteOverflow = true;
					}
					else {
						// Step 2.3b - https://forums.nesdev.com/viewtopic.php?f=3&t=13226
						n++;
						if (n == 65) {
							n = 0;
							allSpritesEvaluated = true;
						}
						m++;
						if (m == 4) {
							m = 0;
						}
					}
				}
			}
			else {
				// Step 2.4
			}
		}
	}
	else if (257 <= cycle && cycle <= 320) {
		if (cycle == 257) hadFirstEmptySprite = false;
		int offset = cycle - 257;
		int sprite = offset / 8;
		int num = offset % 8 + 1;
		if (sprite > numSpritesOAMSL - 1) {
			if (hadFirstEmptySprite) {
				hadFirstEmptySprite = true; // TODO: What is this?
			}


		}
		else {
			switch (num) {
				case 1: // Read Y
				case 2: // Copy tile index
				case 3: // Copy attributes
				case 4: // Copy X
				default: // Read X
					break;
			}
		}
	}
	else if (321 <= cycle && cycle <= 340 || cycle == 0) {
		// Step 4
	}
}
void PPU::ChoosePixel() {
	// fetch bg bit
	// shift shift registers
	// if (every 8 cycles) load new data into these registers (only bg?)
	// decrement all 8 sprite x
	// if sprite x == 0 -> activate
	// if sprite active -> shift shift registers (2 of them)
	//		this output accompanies the data in the sprite's latch to form a pixel
	// Check current pixel for each active sprite (from highest to lowest priority)
	//		First non-transparent pixel moves to multiplexer to join bg pixel
	// Chose pixel
	bool hasSpriteBeenFound = false;
	if (shouldShowBackground || shouldShowSprites) {
		uint8_t bgPxl, sprPxl;
		uint8_t bgAttr, sprAttr;
		bool isSpriteZero = false;
		isSprite0Hit = false;
		if (shouldShowBackground) {
			bgPxl = ((((bsr16Bg[1] & 0x8000) != 0) & 1) << 1) | (((bsr16Bg[0] & 0x8000) != 0) & 1);
			bgAttr = ((((bsr8Bg[1] & 0x80) != 0) & 1) << 1) | (((bsr8Bg[0] & 0x80) != 0) & 1);
		}
		if (shouldShowSprites) {
			for (int i = 0; i < 8; i++) { // Loop through sprites
				if (ctrSpr[i] == 0) { // Sprite active
					if (!hasSpriteBeenFound) {
						sprPxl = ((bsrSpr[i][1] & 0x80) >> 6) | ((bsrSpr[i][0] & 0x80) >> 7);
						sprAttr = lchSpr[i];
						
						// if pixel not transparent
						// if not (color = 0 on palette = 0)
						if (spritePixelsLeft[i] > 0 && !(sprPxl == 0 && ((sprAttr & 0x3) == 0))) {
							hasSpriteBeenFound = true;
							if (i == 0)
								isSpriteZero = true;
						}
					}
					if (spritePixelsLeft[i] > 0)
						spritePixelsLeft[i]--;
					bsrSpr[i][0] = bsrSpr[i][0] << 1;
					bsrSpr[i][1] = bsrSpr[i][1] << 1;
				}
				if (ctrSpr[i] > 0) // Decrement X position
					ctrSpr[i]--;
			}
		}

		// Multiplexer
		uint8_t outPxl, outAttr;
		bool isSprite;

		if (shouldShowBackground && hasSpriteBeenFound) {
			bool priority = sprAttr & (1 << 5);
			if (bgPxl == 0 && sprPxl == 0) {
				// BG ($3F00)
				outPxl = 0;
				outAttr = 0;
				isSprite = false;
			}
			else if (bgPxl == 0) {
				// Sprite
				outPxl = sprPxl;
				outAttr = sprAttr;
				isSprite = true;
			}
			else if (sprPxl == 0) {
				// BG
				outPxl = bgPxl;
				outAttr = bgAttr;
				isSprite = false;
			}
			else {
				if (!priority) {
					// Sprite
					outPxl = sprPxl;
					outAttr = sprAttr;
					isSprite = true;
				}
				else {
					// BG
					outPxl = bgPxl;
					outAttr = bgAttr;
					isSprite = false;
				}
				if (isSpriteZero)
					isSprite0Hit = true;
			}
		}
		else if (shouldShowBackground) {
			// BG
			outPxl = bgPxl;
			outAttr = bgAttr;
			isSprite = false;
		}
		else if (hasSpriteBeenFound) {
			// Sprite
			outPxl = sprPxl;
			outAttr = sprAttr;
			isSprite = true;
		}
		else {
			// TODO: Verify if this guess is accurate
			// BG ($3F00)
			outPxl = 0;
			outAttr = 0;
			isSprite = false;
		}
		RenderPixel(outPxl, outAttr, isSprite);
	}
}
void PPU::RenderPixel(uint8_t outPxl, uint8_t outAttr, bool isSprite) {
	uint16_t colourLoc = 0x3F00;

	if (outPxl != 0) {
		colourLoc += ((isSprite & 1) << 4) + ((outAttr & 3) << 2) + (outPxl & 3);
	}

	uint8_t palColour = Read(colourLoc);
	Colour colour = palette.palette[palColour];

	gls->SetPixel(cycle - 1, scanline, colour.red, colour.green, colour.blue);
}

void PPU::LoadBGTile(int x, int y, int stepNum) {
	// TODO: No alternating between read and write cycles?
	if (stepNum == 1) {
		// Nametable table byte write (and read?)
		bgNTTemp = Read(nameTableAddr + 32 * (y/8) + (x / 8));
	}
	else if (stepNum == 3) {
		// Attribute table byte write (and read?)
		uint8_t temp = Read(nameTableAddr + 0x3c0 + 8 * (y / 32) + (x / 32));
		bool isLeft = (x % 32 < 16);
		bool isTop = (y % 32 < 16);
		// Quadrant Number
		//		01
		//		23
		uint8_t quadrantNum = 0;
		if (!isLeft) quadrantNum++;
		if (!isTop) quadrantNum += 2;
		// TODO: non-zero palette
		nextAttrByte = (temp >> (2 * quadrantNum)) & 3;
	}
	else if (stepNum == 5 || stepNum == 7) {
		// Tile bitmap low and high write (and read?)
		bool isTileLow = stepNum == 5;
		uint8_t tileIndexNum = bgNTTemp;
		uint8_t TileFromTop = (y / 8);
		uint8_t YInTile = y - TileFromTop * 8;
		uint16_t tileAddr = patternTableAddrBackground + (tileIndexNum << 4) + YInTile;
		if (isTileLow) {
			nextTileLow = Read(tileAddr);
		}
		else {
			// TODO: Fix vram
			nextTileHigh = Read(tileAddr + 8); // http://wiki.nesdev.com/w/index.php/PPU_pattern_tables
		}
	}
}
void PPU::FlushBGShifters() {
	// cycle = 9, 17, 25, ..., 257 and 329, 337
	// Source: NTSC Frame Timing Diagram
	if ((shouldShowBackground || shouldShowSprites) && -1 <= scanline && scanline <= 239) {
		if (((1 < cycle && cycle <= 257) || (321 < cycle && cycle <= 337))
			&& ((cycle - 1) % 8 == 0)) {
			bsr16Bg[0] = (bsr16Bg[0] & 0xFF00) | nextTileLow;
			bsr16Bg[1] = (bsr16Bg[1] & 0xFF00) | nextTileHigh;
			attrLatch = nextAttrByte & 3;
		}
	}
}
void PPU::ShiftBGShifters() {
	// cycle = 2 ... 257 and 322 ... 337
	// Source: NTSC Frame Timing Diagram
	if ((shouldShowBackground || shouldShowSprites) && -1 <= scanline && scanline <= 239) {
		if ((1 < cycle && cycle <= 257) || (321 < cycle && cycle <= 337)) {
			bsr16Bg[0] = (bsr16Bg[0] << 1);
			bsr16Bg[1] = (bsr16Bg[1] << 1);
			bsr8Bg[0] = (((bsr8Bg[0] << 1) & 0xFE) | (attrLatch & 1));
			bsr8Bg[1] = (((bsr8Bg[1] << 1) & 0xFE) | ((attrLatch & 2) >> 1));
		}
	}
}
// https://stackoverflow.com/a/2602885
uint8_t PPU::ReverseByte(uint8_t b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}
void PPU::LoadSpritesForScanline() {
	int spriteNum = (cycle - 257) / 8;
	spritePixelsLeft[spriteNum] = 8;
	int stepNum = (cycle - 257) % 8;
	uint8_t spriteY = OAMSL[4 * spriteNum + 0];
	bool shouldFlipHori = (OAMSL[4 * spriteNum + 2] >> 6) & 1;
	bool shouldFlipVert = (OAMSL[4 * spriteNum + 2] >> 7) & 1;
	// TODO: Reorder below using isSpriteOnCurrScanline
	bool isSpriteOnCurrScanline = spriteY <= scanline && scanline < (spriteY + spriteHeight);
	if (stepNum == 2) {
		// Load Attr byte
		if (isSpriteOnCurrScanline)
			lchSpr[spriteNum] = OAMSL[4 * spriteNum + 2];
		else
			lchSpr[spriteNum] = 0;
	}
	else if (stepNum == 3) {
		// Load X cord
		if (isSpriteOnCurrScanline)
			ctrSpr[spriteNum] = OAMSL[4 * spriteNum + 3];
		else
			ctrSpr[spriteNum] = 0;
	}
	else if (stepNum == 5 || stepNum == 7) {
		// Tile bitmap low or high
		// Read on 4 and write on 5?
		// TODO: No alternating between read and write cycles?
		bool isTileLow = stepNum == 5;
		uint8_t tileIndexNum = OAMSL[4 * spriteNum + 1];
		//if (tileIndexNum == 0xee)
			//std::cout << "ASDF";
		uint16_t YInTile = shouldFlipVert ? spriteHeight - (scanline - spriteY) : scanline - spriteY;
		uint16_t tileAddr;
		if (spriteHeight == 8) {
			uint16_t patTabAddr = patternTableAddrSprite;
			tileAddr = patTabAddr + (tileIndexNum << 4) + YInTile;
		}
		else {
			uint16_t patTabAddr = (tileIndexNum & 1) ? 0x1000 : 0x0000;
			tileAddr = patTabAddr + ((tileIndexNum & 0xFE) << 4) + YInTile;
		}
		if (isTileLow) {
			if (isSpriteOnCurrScanline) {
				if (!shouldFlipHori)
					bsrSpr[spriteNum][0] = Read(tileAddr);
				else
					bsrSpr[spriteNum][0] = ReverseByte(Read(tileAddr));
			}
			else
				bsrSpr[spriteNum][0] = 0;
		}
		else {
			if (isSpriteOnCurrScanline) {
				if (!shouldFlipHori)
					bsrSpr[spriteNum][1] = Read(tileAddr + 8); // http://wiki.nesdev.com/w/index.php/PPU_pattern_tables
				else
					bsrSpr[spriteNum][1] = ReverseByte(Read(tileAddr + 8)); // http://wiki.nesdev.com/w/index.php/PPU_pattern_tables
			}
			else
				bsrSpr[spriteNum][1] = 0;
		}
	}
}

// http://wiki.nesdev.com/w/index.php/PPU_rendering#Line-by-line_timing
// Each 
void PPU::RenderTick() {
	if (scanline == -1) {
		// Pre-render scanline
		if (cycle == 1) {
			hasNotifiedVBlank = false;
			isInVBlank = false;
		}
		else if (321 <= cycle && cycle <= 336) {
			// Load first 2 background tiles for next scanline into registers
			int stepNum = (cycle - 321) % 8;
			LoadBGTile(cycle - 321, scanline + 1, stepNum);
		}
	}
	else if ((shouldShowBackground || shouldShowSprites) && 0 <= scanline && scanline <= 239) {
		// Visible scanlines
		if (cycle == 0) {
			// Idle
		}
		else if (1 <= cycle && cycle <= 256) {
			int stepNum = (cycle - 1) % 8;
			LoadBGTile(cycle + 16, scanline, stepNum);
		}
		else if (257 <= cycle && cycle <= 320) {
			// Load Sprites into registers
			LoadSpritesForScanline();
		}
		else if (321 <= cycle && cycle <= 336) {
			// Load Background tiles into registers
			int stepNum = (cycle - 321) % 8;
			LoadBGTile(cycle - 321, scanline + 1, stepNum);
		}
		else if (337 <= cycle && cycle <= 340) {
			// Load 2 nametable bytes
		}
	}
	else if (scanline == 240) {
		// Post-render scanline - PPU idles
	}
	else if (241 <= scanline && scanline <= 260) {
		if (cycle == 1 && scanline == 241) {
			isInVBlank = true;
		}
	}
}
void PPU::Tick() {
	// TODO: Check order of these functions
	RenderTick();
	ShiftBGShifters();
	FlushBGShifters();
	if ((shouldShowBackground || shouldShowSprites) && (0 <= scanline && scanline <= 239)) {
		if (shouldShowSprites)
			SpriteEvaluation();
		if (1 <= cycle && cycle <= 256)
			ChoosePixel();
	}

	// Update scanline and cycle counters
	if (scanline == -1 && isOddFrame && cycle == 339) {
		cycle = 0;
	}
	else {
		cycle = (cycle + 1) % 341;
	}
	if (cycle == 0) {
		scanline = ((scanline + 1 + 1) % 262) - 1;
	}
	bool nmiState = VBlankShouldNMI && isInVBlank;
	cpuNMIConnection.SetState(nmiState);
}

void PPU::PowerUp() {
	PPUCTRL(0);
	PPUMASK(0);
	PPUSTATUS();
	OAMADDR(0);
	PPUSCROLL(0);
	PPUADDR(0);
}

PPU::PPU(CPU_NES* _CPUPtr, GamePak* _gp, GLScene2D* _gls) :
	PaletteRAM {
		0x09, 0x01, 0x00, 0x01,
		0x00, 0x02, 0x02, 0x0D,
		0x08, 0x10, 0x08, 0x24,
		0x00, 0x00, 0x04, 0x2C,
		0x09, 0x01, 0x34, 0x03,
		0x00, 0x04, 0x00, 0x14,
		0x08, 0x3A, 0x00, 0x02,
		0x00, 0x20, 0x2C, 0x08
	}
{
	CPUPtr = _CPUPtr;
	PowerUp();
	CPUPtr->AddNMIConnection(&cpuNMIConnection);
	gp = _gp;
	gls = _gls;
}