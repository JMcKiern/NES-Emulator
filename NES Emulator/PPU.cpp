#include <exception>
#include "PPU.h"
#include "Colour.h"

// PPU Memory
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

void PPU::WriteReg(uint16_t offset, uint8_t data) {
	lastWrite = data;
	switch (offset) {
		case 0x2000: PPUCTRL(data); break;
		case 0x2001: PPUMASK(data); break;
		case 0x2003: OAMADDR(data); break;
		case 0x2004: OAMDATA(data); break;
		case 0x2005: PPUSCROLL(data); break;
		case 0x2006: PPUADDR(data); break;
		case 0x2007: PPUDATA(data); break;
		case 0x4014: OAMDMA(data); break;
		default:
			throw std::out_of_range::out_of_range("Attempted PPU write at " + offset);
			break;
	}
}
uint8_t PPU::ReadReg(uint16_t offset) {
	switch (offset) {
		case 0x2002: return PPUSTATUS();
		case 0x2004: return OAMDATA();
		case 0x2007: return PPUDATA();
		default:
			throw std::out_of_range::out_of_range("Attempted PPU read at " + offset);
			return -1;
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
	uint8_t status = 0;
	status |= (lastWrite & 0x1F);
	status |= ((isSpriteOverflow & 0x1) << 5);
	status |= ((isSprite0Hit & 0x1) << 6);
	status |= ((isInVBlank & 0x1) << 7);
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
	if (1 <= cycle && cycle <= 64) return 0xFF;
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
	VRAMPtr += addrInc;
	return data;
}
void PPU::OAMDMA(uint8_t data) {
	for (int i = 0; i < 0x100; i++) {
		OAM[i] = CPUPtr->PPURequestingRead((data * 0x100) + i);
	}
}

// https://forums.nesdev.com/viewtopic.php?f=3&t=13226
// http://wiki.nesdev.com/w/index.php/PPU_sprite_evaluation
void PPU::SpriteEvaluation() {
	// Move These
	int n = 0;
	int m = 0;
	bool isOddCycle = cycle % 2 == 1;
	int numSpritesOAMSL = 0;
	bool allSpritesEvaluated = false;
	bool hadFirstEmptySprite = false;
	// !!

	if (cycle == 0) {

	}
	if (1 <= cycle && cycle <=64) {
		if (isOddCycle) {
			spriteEvalTemp = OAMDATA();
		}
		else {
			OAMSL[oamSLAddr++] = spriteEvalTemp;
		}
	}
	else if (65 <= cycle && cycle <= 256) {
		if (isOddCycle) {
			// Read
			//spriteEvalTemp = OAM[oamAddr++];
			spriteEvalTemp = OAM[n * 4 + m];
		}
		else {
			// Write
			if (!allSpritesEvaluated) {
				if (numSpritesOAMSL < 8) { // Step 2.2b
					if (m == 0) {
						// Step 2.1
						OAMSL[oamSLAddr] = spriteEvalTemp;
						m++;
					}
					else {
						uint8_t spriteY = OAMSL[oamSLAddr - m];
						if (spriteY <= scanline && // Should copy sprite
							scanline <= (spriteY + spriteHeight)) {
							// Step 2.1a
							numSpritesOAMSL++;
							oamSLAddr++;
							OAMSL[oamSLAddr] = spriteEvalTemp;
							m++;
							if (m == 4) { // Finished copying sprite
								// Step 2.2
								n++;
								if (n == 0) {
									// Step 2.2a
									allSpritesEvaluated = true;
								}
								m = 0;
								oamSLAddr++;
							}
						}
						else { // Sprite not in range
							n++;
							if (n == 0) {
								allSpritesEvaluated = true;
							}
							m = 0;
						}
					}
				}
				else {
					// Step 2.2c
					// Step 2.3
					uint8_t spriteY = OAMSL[oamSLAddr - m];
					if (spriteY <= scanline && // Should copy sprite
						scanline <= (spriteY + spriteHeight)) {
						// Step 2.3a
						isSpriteOverflow = true;
					}
					else {
						// Step 2.3b - https://forums.nesdev.com/viewtopic.php?f=3&t=13226
						n++;
						m++;
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
				hadFirstEmptySprite = true;
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
void PPU::RenderPixel() {
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
	uint8_t bgPxl, sprPxl;
	uint8_t bgAttr, sprAttr;
	if (shouldShowBackground) {
		bgPxl = (bsr16Bg[0] & 3); // = ?? // TODO: this is incorrect
		bsr16Bg[0] = (bsr16Bg[0] >> 1) & ((bsr16Bg[1] & 1) << 7);
		bsr16Bg[1] = (bsr16Bg[1] >> 1); // Could be simplified if using one bsr for bg bitmap
		bgAttr = (bsr8Bg[0] & 1);
		bsr8Bg[0] = (bsr8Bg[0] >> 1) & ((bsr8Bg[1] & 1) << 7);
		bsr8Bg[1] = (bsr8Bg[1] >> 1); // Could be simplified if using one bsr for bg pal attr
		if ((cycle > 1) && ((cycle - 1) % 8 == 0)) {  // cycle = 9, 17, 25, ..., 257
			// load bitmap data to bsr16[1] at bits 8 to 15 (upper 8 bits)
			// load palette attributes
		}
	}
	if (shouldShowSprites) {
		for (int i = 0; i < 8; i++) { // Loop through sprites
			if (ctrSpr[i] > 0) // Decrement X position
				ctrSpr[i]--;
			if (ctrSpr[i] == 0) { // Sprite active
				sprPxl = ((bsrSpr[i][1] & 1) << 1) & (bsrSpr[i][0] & 1); // TODO: Correct? 
				bsrSpr[i][0] = bsrSpr[i][0] >> 1;
				bsrSpr[i][1] = bsrSpr[i][2] >> 1;
				sprAttr = lchSpr[i] >> 1;
				lchSpr[i] = lchSpr[i] >> 1;

				if (pixel not transparent) {
					// Flip sprites?
					break;
				}
			}
		}
	}

	// Multiplexer
	{
		if (shouldShowBackground && shouldShowSprites) {
			bool spritePriority = sprAttr & (1 << 5);
			if (bgPxl == 0 && sprPxl == 0) {
				// BG ($3F00)
			}
			else if (bgPxl == 0) {
				// Sprite
				outPxl = sprPxl;
				outAttr = sprAttr;
			}
			else if (sprPxl == 0) {
				// BG
				outPxl = bgPxl;
				outAttr = bgAttr;
			}
			else {
				if (spritePriority) {
					// Sprite
					outPxl = sprPxl;
					outAttr = sprAttr;
				}
				else {
					// BG
					outPxl = bgPxl;
					outAttr = bgAttr;
				}
			}
		}
		else if (shouldShowBackground) {
			// BG
			outPxl = bgPxl;
			outAttr = bgAttr;
		}
		else if (shouldShowSprites) {
			// Sprite
			outPxl = sprPxl;
			outAttr = sprAttr;
		}
		else {

		}
	}
	
}
// http://wiki.nesdev.com/w/index.php/PPU_rendering#Line-by-line_timing
// Each 
void PPU::RenderTick() {
	if (scanline == -1) {
		// Pre-render scanline

	}
	else if (0 <= scanline && scanline <= 239) {
		if (cycle == 0) {

		}
		else if (1 <= cycle && cycle <= 256) {

		}
		else if (257 <= cycle && cycle <= 320) {
			// Load Sprites into registers

		}
		else if (321 <= cycle && cycle <= 336) {
			// Load Background tiles into registers
		}
		else if (337 <= cycle && cycle <= 340) {
			// Load 2 nametable bytes
		}
	}
	else if (scanline == 240) {
		// Post-render scanline - PPU idles
	}
	else if (241 <= scanline && scanline <= 260) {
		if (cycle == 1 && scanline == 240) {
			isInVBlank = true;
		}
	}
}
void PPU::Tick() {
	bool nmiState = VBlankShouldNMI && isInVBlank;
	cpuNMIConnection.SetState(nmiState);

	RenderTick();
	RenderPixel();
	SpriteEvaluation();

	// Update scanline and cycle counters
	if (scanline == -1 && isOddFrame && cycle == 339) {
		cycle = 0;
	}
	else {
		cycle = (cycle + 1) % 341;
	}
	scanline = ((scanline + 1) % 261) - 1;
}

void PPU::PowerUp() {
	PPUCTRL(0);
	PPUMASK(0);
	PPUSTATUS();
	OAMADDR(0);
	PPUSCROLL(0);
	PPUADDR(0);
}

PPU::PPU(CPU_NES* _CPUPtr) {
	CPUPtr = _CPUPtr;
	PowerUp();
	CPUPtr->AddNMIConnection(&cpuNMIConnection);
}