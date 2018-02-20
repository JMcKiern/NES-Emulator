#pragma once

#include "Colour.h"
struct Palette {
	Colour palette[0x40];

	Palette() :
		palette{
			Colour(0x75, 0x75, 0x75),
			Colour(0x27, 0x1B, 0x8F),
			Colour(0x00, 0x00, 0xAB),
			Colour(0x47, 0x00, 0x9F),
			Colour(0x8F, 0x00, 0x77),
			Colour(0xAB, 0x00, 0x13),
			Colour(0xA7, 0x00, 0x00),
			Colour(0x7F, 0x0B, 0x00),
			Colour(0x43, 0x2F, 0x00),
			Colour(0x00, 0x47, 0x00),
			Colour(0x00, 0x51, 0x00),
			Colour(0x00, 0x3F, 0x17),
			Colour(0x1B, 0x3F, 0x5F),
			Colour(0x00, 0x00, 0x00),
			Colour(0x00, 0x00, 0x00),
			Colour(0x00, 0x00, 0x00),
			Colour(0xBC, 0xBC, 0xBC),
			Colour(0x00, 0x73, 0xEF),
			Colour(0x23, 0x3B, 0xEF),
			Colour(0x83, 0x00, 0xF3),
			Colour(0xBF, 0x00, 0xBF),
			Colour(0xE7, 0x00, 0x5B),
			Colour(0xDB, 0x2B, 0x00),
			Colour(0xCB, 0x4F, 0x0F),
			Colour(0x8B, 0x73, 0x00),
			Colour(0x00, 0x97, 0x00),
			Colour(0x00, 0xAB, 0x00),
			Colour(0x00, 0x93, 0x3B),
			Colour(0x00, 0x83, 0x8B),
			Colour(0x00, 0x00, 0x00),
			Colour(0x00, 0x00, 0x00),
			Colour(0x00, 0x00, 0x00),
			Colour(0xFF, 0xFF, 0xFF),
			Colour(0x3F, 0xBF, 0xFF),
			Colour(0x5F, 0x97, 0xFF),
			Colour(0xA7, 0x8B, 0xFD),
			Colour(0xF7, 0x7B, 0xFF),
			Colour(0xFF, 0x77, 0xB7),
			Colour(0xFF, 0x77, 0x63),
			Colour(0xFF, 0x9B, 0x3B),
			Colour(0xF3, 0xBF, 0x3F),
			Colour(0x83, 0xD3, 0x13),
			Colour(0x4F, 0xDF, 0x4B),
			Colour(0x58, 0xF8, 0x98),
			Colour(0x00, 0xEB, 0xDB),
			Colour(0x00, 0x00, 0x00),
			Colour(0x00, 0x00, 0x00),
			Colour(0x00, 0x00, 0x00),
			Colour(0xFF, 0xFF, 0xFF),
			Colour(0xAB, 0xE7, 0xFF),
			Colour(0xC7, 0xD7, 0xFF),
			Colour(0xD7, 0xCB, 0xFF),
			Colour(0xFF, 0xC7, 0xFF),
			Colour(0xFF, 0xC7, 0xDB),
			Colour(0xFF, 0xBF, 0xB3),
			Colour(0xFF, 0xDB, 0xAB),
			Colour(0xFF, 0xE7, 0xA3),
			Colour(0xE3, 0xFF, 0xA3),
			Colour(0xAB, 0xF3, 0xBF),
			Colour(0xB3, 0xFF, 0xCF),
			Colour(0x9F, 0xFF, 0xF3),
			Colour(0x00, 0x00, 0x00),
			Colour(0x00, 0x00, 0x00),
			Colour(0x00, 0x00, 0x00)
	}
	{}
};