#include <iomanip>
#include <sstream>
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
	if (0x2400 <= offset && offset <= 0x2FFF) { // Nametable 1, 2 or 3
		offset = (*mapperPtrPtr)->UnMirror(offset);
	}
	return offset;
}
void PPU::Write(uint16_t offset, uint8_t data) {
	offset = UnMirror(offset);
	if (0x3f00 <= offset && offset <= 0x3f20) {
		// Palette Ram
		PaletteRAM[offset - 0x3f00] = data;
	}
	else {
		if ((*mapperPtrPtr)->isVRAM(offset)) {
			uint16_t vramOffset = (*mapperPtrPtr)->GetVRAMAddr();
			*(vram + (offset - vramOffset)) = data;
		}
		else {
			(*mapperPtrPtr)->PPUWrite(offset, data);
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
		if ((*mapperPtrPtr)->isVRAM(offset)) {
			uint16_t vramOffset = (*mapperPtrPtr)->GetVRAMAddr();
			return vram[offset - vramOffset];
		}
		else {
			return (*mapperPtrPtr)->PPURead(offset);
		}
	}
}

void PPU::UpdateA12(uint16_t addr) {
	static uint8_t prevA12;
	uint8_t currA12 = (addr >> 12) & 0x1;
	// Need to correct dummy fetches for this to work
	//if (prevA12 == 0 && currA12 == 1) {
	// Quick fix
	if (cycle == 256) {
		// PPU A12 0->1
		if ((*mapperPtrPtr)->GetMapperNum() == 4) {
			(*mapperPtrPtr)->IRQClock();
		}
	}
	prevA12 = currA12;
}

// PPU Registers
void PPU::PPUCTRL(uint8_t data) {
	uint8_t nameTable = (data & 0x3);
	if (nameTable == 0) nameTableAddr = 0x2000;
	else if (nameTable == 1) nameTableAddr = 0x2400;
	else if (nameTable == 2) nameTableAddr = 0x2800;
	else if (nameTable == 3) nameTableAddr = 0x2C00;

	// t: ...BA.. ........ = d: ......BA
	t = (t & ~(0x3 << 10)) | ((data & 0x3) << 10);

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
	colourEmphasis = ((data >> 5) & 0x7);
}
uint8_t PPU::PPUSTATUS() { // Potential Error: 5 LSB should not be zero
	w = 0; // Reset data bus
	uint8_t status = 0;
	status |= (openBus & 0x1F);
	status |= ((isSpriteOverflow & 0x1) << 5);
	status |= ((isSprite0Hit & 0x1) << 6);
	status |= ((isInVBlank & 0x1) << 7);
	isInVBlank = false;
	// Reading 1 PPU clock before VBL should suppress setting
	if (cycle == 0 && scanline == 241) {
	//if (cycle == 340 && scanline == 240) {
		// This must be cycle == 0(1) since cycle corresponds to the cycle on
		// prev(next) PPU tick
		shouldSuppressSettingVBL = true;
	}
	// Reading flag when it's set should suppress NMI
	if (cycle < 3 && scanline == 241) {
		//shouldSuppressNMI = true;
	}
	openBus = status;
	return status;
}
void PPU::OAMADDR(uint8_t data) {
	oamAddr = data;
}
void PPU::OAMDATA(uint8_t data) {
	if (oamAddr % 4 == 2)
		OAM[oamAddr] = (data & ~0x1c);
	else
		OAM[oamAddr] = data;
	oamAddr++;
}
uint8_t PPU::OAMDATA() {
	openBus = OAM[oamAddr];
	return OAM[oamAddr];// returnValue;
}
void PPU::PPUSCROLL(uint8_t data) {
	if (w == 0) { // isNextScrollX = true
		// t: ....... ...HGFED = d: HGFED...
		t = (t & ~0x1F) | ((data >> 3) & 0x1F);

		// x:              CBA = d: .....CBA
		x = data & 0x7;

		w = 1;
	}
	else { // w == 1
		// t: CBA.... ........ = d: .....CBA
		t = (t & ~(0x7 << 12)) | ((data & 0x7) << 12);

		// t: .....HG FED..... = d: HGFED...
		t = (t & ~(0x1F << 5)) | (((data >> 3) & 0x1F) << 5);

		w = 0;
	}
}
void PPU::PPUADDR(uint8_t data) {
	if (w == 0) { // isNextByteUpper = true
		// t: .FEDCBA ........ = d: ..FEDCBA
		t = (t & ~(0x3F << 8)) | ((data & 0x3F) << 8);

		// t: X...... ........ = 0
		t = (t & ~(0x1 << 14));

		w = 1;
	}
	else {
		// t: ....... HGFEDCBA = d: HGFEDCBA
		t = (t & ~0xFF) | data;

		v = t;
		w = 0;
	}
}
void PPU::PPUDATA(uint8_t data) {
	Write(v, data);
	if (-1 <= scanline && scanline <= 239
	    && (shouldShowBackground || shouldShowSprites)) {
		ScrlCoarseXInc();
		ScrlYInc();
	}
	else {
		v += addrInc;
	}
}
uint8_t PPU::PPUDATA() {
	uint8_t returnValue;
	if (0x3F00 <= v && v <= 0x3FFF) { // Palette RAM
		ppudataReadBuffer = Read(v - 0x1000);
		returnValue = Read(v);
		returnValue |= (openBus & 0xc0);
	}
	else {
		returnValue = ppudataReadBuffer;
		ppudataReadBuffer = Read(v);
	}
	openBus = returnValue;
	if (-1 <= scanline && scanline <= 239
	    && (shouldShowBackground || shouldShowSprites)) {
		ScrlCoarseXInc();
		ScrlYInc();
	}
	else {
		v += addrInc;
	}
	return returnValue;
}
void PPU::OAMDMA(uint8_t data) {
	for (int i = 0; i < 0x100; i++) {
		uint8_t byte = CPUPtr->PPURequestingRead((data * 0x100) + i);
		CPUPtr->PPURequestingWrite(0x2004, byte);
	}
}

// https://forums.nesdev.com/viewtopic.php?f=3&t=13226
// http://wiki.nesdev.com/w/index.php/PPU_sprite_evaluation
void PPU::SpriteEvaluation() {
	if (!(0 <= scanline && scanline <= 239))
		return;
	static int n = 0; // TODO: Move to class
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
			// TODO: It is unclear whether OAMDATA() should return 0xFF
			// (as in Sprite Evaluation page on wiki) or if there
			// should be no OAMDATA() call and just set it to 0xFF
			// In order to make sprite_ram.nes work the 2nd option
			// should be chosen.
			spriteEvalTemp = 0xFF;
		}
		else {
			// TODO: This should probably set byte 2 to 0xE3
			OAMSL[oamSLAddr] = spriteEvalTemp;
			oamSLAddr = (oamSLAddr + 1) % 0x20;
		}
	}
	else if (65 <= cycle && cycle <= 256) {
		bool isOddCycle = cycle % 2 == 1;
		if (isOddCycle) {
			// Read
			// TODO: Use oamAddr (Check PPU Registers wiki page)
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
						// Step 2.3b
						// https://forums.nesdev.com/viewtopic.php?f=3&t=13226
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
	else if ((321 <= cycle && cycle <= 340) || cycle == 0) {
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
	//		this output accompanies the data in the sprite's latch to form
	//		 a pixel
	// Check current pixel for each active sprite (from highest to lowest
	//	priority)
	//		First non-transparent pixel moves to multiplexer to join bg pixel
	// Chose pixel
	bool hasSpriteBeenFound = false;
	bool isSpriteZero = false;
	if (shouldShowBackground || shouldShowSprites) {
		uint8_t bgPxl = 0, sprPxl = 0;
		uint8_t bgAttr = 0, sprAttr = 0;
		if (shouldShowBackground) {
			bgPxl = ((((bsr16Bg[1] & (0x8000 >> x)) != 0) & 1) << 1)
			        | (((bsr16Bg[0] & (0x8000 >> x)) != 0) & 1);
			bgAttr = ((((bsr8Bg[1] & (0x80 >> x)) != 0) & 1) << 1)
			         | (((bsr8Bg[0] & (0x80 >> x)) != 0) & 1);

			// Clip background in leftmost 8 pixels of screen
			if (shouldClipBackground && 1 <= cycle && cycle <= 8) {
				bgPxl = 0;
				bgAttr = 0;
			}
		}
		if (shouldShowSprites) {
			for (int i = 0; i < 8; i++) { // Loop through sprites
				if (ctrSpr[i] == 0) { // Sprite active
					if (!hasSpriteBeenFound) {
						sprPxl = ((bsrSpr[i][1] & 0x80) >> 6)
						         | ((bsrSpr[i][0] & 0x80) >> 7);
						sprAttr = lchSpr[i];

						// if pixel not transparent
						// if not (color = 0 on palette = 0)
						if (spritePixelsLeft[i] > 0
						    && !(sprPxl == 0 && ((sprAttr & 0x3) == 0))) {
							hasSpriteBeenFound = true;
							if (i == 0)
								isSpriteZero = true;
						}
					}
					//if (spritePixelsLeft[i] > 0)
						//spritePixelsLeft[i]--;
					//bsrSpr[i][0] = bsrSpr[i][0] << 1;
					//bsrSpr[i][1] = bsrSpr[i][1] << 1;
				}
				//if (ctrSpr[i] > 0) // Decrement X position
					//ctrSpr[i]--;
			}
			// Clip sprites in leftmost 8 pixels of screen
			if ((shouldClipSprites && 1 <= cycle && cycle <= 8)) {
				hasSpriteBeenFound = false;
				isSpriteZero = false;
				sprPxl = 0;
				sprAttr = 0;
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
				if (hasSpriteBeenFound && isSpriteZero) {
					// Sprite 0 hit does not happen: At x=255 or y>239
					if (!(cycle == 256)){// || scanline >= 239)) {
						isSprite0Hit = true;
					}
				}
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
		//if (0 <= scanline && scanline <= 239)
			RenderPixel(outPxl, outAttr, isSprite);
	}
}
void PPU::RenderPixel(uint8_t outPxl, uint8_t outAttr, bool isSprite) {
	uint16_t colourLoc = 0x3F00;

	if (outPxl != 0) {
		colourLoc += ((isSprite & 1) << 4) + ((outAttr & 3) << 2)
		             + (outPxl & 3);
	}

	uint8_t palColour = Read(colourLoc);
	Colour colour = palette.GetColour(palColour, colourEmphasis);

	disp[cycle - 1][scanline] = palColour;
	gls->SetPixel(cycle - 1, scanline, colour.red, colour.green, colour.blue);
}

void PPU::LoadBGTile(int stepNum) {
	// TODO: No alternating between read and write cycles?
	if (stepNum == 1) {
		// Nametable table byte write (and read?)
		uint16_t addr = 0x2000 | (v & 0x0FFF);
		bgNTTemp = Read(addr);
	}
	else if (stepNum == 3) {
		// Attribute table byte write (and read?)
		uint16_t tempAddr = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38)
		                    | ((v >> 2) & 0x07);
		uint8_t temp = Read(tempAddr);
		uint8_t shift = ((v >> 4) & 0x04) | (v & 0x02);
		nextAttrByte = ((temp >> shift) & 0x03);
	}
	else if (stepNum == 5 || stepNum == 7) {
		// Tile bitmap low and high write (and read?)
		bool isTileLow = stepNum == 5;
		uint8_t tileIndexNum = bgNTTemp;
		uint16_t tileAddr = patternTableAddrBackground | (tileIndexNum << 4)
		                    | (v >> 12);
		if (isTileLow) {
			nextTileLow = Read(tileAddr);
		}
		else {
			// http://wiki.nesdev.com/w/index.php/PPU_pattern_tables
			nextTileHigh = Read(tileAddr + 8);
		}
	}
}
void PPU::FlushBGShifters() {
	// cycle = 9, 17, 25, ..., 257 and 329, 337
	// Source: NTSC Frame Timing Diagram
	if ((shouldShowBackground || shouldShowSprites)
	    && -1 <= scanline && scanline <= 239) {
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
	if ((shouldShowBackground || shouldShowSprites)
	    && -1 <= scanline && scanline <= 239) {
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
	bool isSpriteOnCurrScanline = spriteY <= scanline
	                              && scanline < (spriteY + spriteHeight);
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
		uint16_t YInTile = shouldFlipVert
		                   ? spriteHeight - (scanline - spriteY) - 1
		                   : scanline - spriteY;
		uint8_t secondTile8x16Offset = YInTile >= 8 ? 8 : 0;
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
					bsrSpr[spriteNum][0] = Read(tileAddr + secondTile8x16Offset);
				else
					bsrSpr[spriteNum][0] = ReverseByte(Read(tileAddr + secondTile8x16Offset));
			}
			else
				bsrSpr[spriteNum][0] = 0;
		}
		else {
			if (isSpriteOnCurrScanline) {
				// http://wiki.nesdev.com/w/index.php/PPU_pattern_tables
				if (!shouldFlipHori)
					bsrSpr[spriteNum][1] = Read(tileAddr + 8 + secondTile8x16Offset);
				else
					bsrSpr[spriteNum][1] = ReverseByte(Read(tileAddr + 8 + secondTile8x16Offset));
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
			isInVBlank = false;
			// Bit 7, 6 and 5 of PPUSTATUS ($2002) is cleared at dot 1 of the
			// pre-render line.
			isSprite0Hit = false;
			isSpriteOverflow = false;
		}
		else if (321 <= cycle && cycle <= 336) {
			// Load first 2 background tiles for next scanline into registers
			int stepNum = (cycle - 321) % 8;
			LoadBGTile(stepNum);
		}
	}
	else if ((shouldShowBackground || shouldShowSprites)
	         && 0 <= scanline && scanline <= 239) {
		UpdateA12(0x0);
		// Visible scanlines
		if (cycle == 0) {
			// Idle
		}
		else if (1 <= cycle && cycle <= 256) {
			int stepNum = (cycle - 1) % 8;
			LoadBGTile(stepNum);
		}
		else if (257 <= cycle && cycle <= 320) {
			// Load Sprites into registers
			LoadSpritesForScanline();
		}
		else if (321 <= cycle && cycle <= 336) {
			// Load Background tiles into registers
			int stepNum = (cycle - 321) % 8;
			LoadBGTile(stepNum);
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
			if (!shouldSuppressSettingVBL) {
				isInVBlank = true;
			}
			shouldSuppressSettingVBL = false;
		}
	}
	// Scrolling behaviour
	if ((-1 <= scanline && scanline <= 239)
	    && (shouldShowBackground || shouldShowSprites)) {
		if (cycle == 256) { // dot 256
			// Increment vertical position in v
			ScrlYInc();
		}
		if (cycle == 257) { // dot 257
			// v: ....... ...EDCBA = t : ....... ...EDCBA
			v = (v & ~(0x1F)) | ((t & 0x1F));

			// v: ....F.. ........ = t : ....F.. ........
			v = (v & ~(0x1 << 10)) | (t & (0x1 << 10));
		}
		if (scanline == -1) {
			if (280 <= cycle && cycle <= 304) { // dot 280-304
				// v: IHGF... ........ = t: IHGF... ........
				v = (v & ~(0xF << 11)) | ((t & (0xF << 11)));

				// v: .....ED CBA..... = t: .....ED CBA.....
				v = (v & ~(0x1F << 5)) | ((t & (0x1F << 5)));
			}
		}
		if (cycle <= 256 || cycle >= 328) {
			// it begins at dots 328 and 336, and will continue through the
			// next scanline at 8, 16, 24... 240, 248, 256 (every 8 dots
			// accross the scanline until 256).
			if (cycle % 8 == 0 && cycle != 0)
				ScrlCoarseXInc();
		}
	}
}
void PPU::ScrlCoarseXInc() {
	// https://wiki.nesdev.com/w/index.php/PPU_scrolling#Coarse_X_increment
	if ((v & 0x001F) == 31) { // if coarse X == 31
		v &= ~0x001F;         // coarse X = 0
		v ^= 0x0400;          // switch horizontal nametable
	}
	else {
		v += 1;               // increment coarse X
	}
}
void PPU::ScrlYInc() {
	// https://wiki.nesdev.com/w/index.php/PPU_scrolling#Y_increment
	if ((v & 0x7000) != 0x7000) {       // if fine Y < 7
		v += 0x1000;                    // increment fine Y
	}
	else {
		v &= ~0x7000;                   // fine Y = 0
		int y = (v & 0x03E0) >> 5;      // let y = coarse Y
		if (y == 29) {
			y = 0;                      // coarse Y = 0
			v ^= 0x0800;                // switch vertical nametable
		}
		else if (y == 31) {
			y = 0;                      // coarse Y = 0, nametable not switched
		}
		else {
			y += 1;                     // increment coarse Y
		}
		v = (v & ~0x03E0) | (y << 5);   // put coarse Y back into v
	}
}
void PPU::ShiftSprShifters() {
	// TODO: Are the sprite bsr's shifted even when not in visible pixels
	if (1 <= cycle && cycle <= 256 && 0 <= scanline && scanline <= 239) {
		for (int i = 0; i < 8; i++) {
			if (ctrSpr[i] == 0) { // Sprite active
				bsrSpr[i][0] = bsrSpr[i][0] << 1;
				bsrSpr[i][1] = bsrSpr[i][1] << 1;
				if (spritePixelsLeft[i] > 0)
					spritePixelsLeft[i]--;
			}
			if (ctrSpr[i] > 0) // Decrement X position
				ctrSpr[i]--;
		}
	}
}

std::string PPU::GetDispHash() {
	// https://gist.github.com/nitrix/34196ff0c93fdfb01d51
	uint32_t magic = 5381;
	for (int y = 0; y < dispHeight; y++) {
		for (int x = 0; x < dispWidth; x++) {
			uint8_t c = disp[x][y];
			magic = ((magic << 5) + magic) + c; // magic * 33 + c
		}
	}
	std::stringstream ss;
	ss << std::hex << std::setw(8) << std::setfill('0') << magic;
	return ss.str();
}
void PPU::Tick() {
	// Update scanline and cycle counters
	if (scanline == -1 && isOddFrame && cycle == 339
	    && (shouldShowBackground || shouldShowSprites)) {
		cycle = 0;
	}
	else {
		cycle = (cycle + 1) % 341;
	}
	if (cycle == 0) {
		scanline = ((scanline + 1 + 1) % 262) - 1;
	}
	if (cycle == 0 && scanline == 0) {
		// Start of new frame
		isOddFrame = !isOddFrame;
		shouldSuppressNMI = false;
		frameCount++;
	}

	// TODO: Check order of these functions
	RenderTick();
	ShiftBGShifters();
	FlushBGShifters();
	if ((shouldShowBackground || shouldShowSprites)
	    && (0 <= scanline && scanline <= 239)) {
		if (shouldShowSprites)
			SpriteEvaluation();
		if (1 <= cycle && cycle <= 256)
			ChoosePixel();
	}
	ShiftSprShifters();

	bool isInVBlankNextCycle = false; // scanline == 241 && cycle == 0;
	if (VBlankShouldNMI && (isInVBlank || isInVBlankNextCycle)
	    && !shouldSuppressNMI) {
		cpuNMIConnection.SetState(LOW);
	}
	else {
		cpuNMIConnection.SetState(HIGH);
	}
}

void PPU::PowerUp() {
	PPUCTRL(0);
	PPUMASK(0);
	PPUSTATUS();
	OAMADDR(0);
	PPUSCROLL(0);
	PPUADDR(0);
}

PPU::PPU(CPU_NES* _CPUPtr, Mapper** _mapperPtrPtr, GLScene2D* _gls) :
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
	mapperPtrPtr = _mapperPtrPtr;
	gls = _gls;

	for (int i = 0; i < 0x100; i++) {
		if (i % 4 == 2)
			OAM[i] = 0xE3;
		else
			OAM[i] = 0xFF;
	}
	for (int i = 0; i < 0x20; i++) {
		if (i % 4 == 2)
			OAMSL[i] = 0xE3;
		else
			OAMSL[i] = 0xFF;
	}
	for (int i = 0; i < 8; i++) {
		bsrSpr[i][0] = 0;
		bsrSpr[i][1] = 0;
		lchSpr[i] = 0;
		ctrSpr[i] = 0;
		spritePixelsLeft[i] = 0;
	}
}

// Frame Check
bool PPU::IsOddFrame() {
	return isOddFrame;
}

// PPU Registers Access Functions for the CPU
void PPU::WriteReg(uint16_t offset, uint8_t data) {
	openBus = data;
	openBusLastRefresh = frameCount;
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
	long framesSinceLastRefresh = frameCount - openBusLastRefresh;
	// Open bus goes to 0 after 600 ms
	if (framesSinceLastRefresh > 0.600 * 60)
		openBus = 0;
	switch (offset) {
		case 0x2002: return PPUSTATUS();
		case 0x2004: return OAMDATA();
		case 0x2007: return PPUDATA();
		default:
			w = 0; // Reset data bus
			return openBus;
	}
}
