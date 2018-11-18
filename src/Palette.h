#pragma once

#include <string>
#include "Colour.h"

class Palette {
private:
	uint8_t palette[0x600];
	void LoadPal(std::string palFile);

public:
	Colour GetColour(uint8_t colourNum, uint8_t colourEmp);
	Palette(std::string palFile);
	Palette();
};