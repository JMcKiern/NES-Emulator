#pragma once

#include <cstdint>

struct Colour {
	uint8_t red, green, blue;

	void operator=(const Colour& other) {
		red = other.red;
		green = other.green;
		blue = other.blue;
	}
	Colour() {}
	Colour(uint8_t _r, uint8_t _g, uint8_t _b) :
		red(_r),
		green(_g),
		blue(_b)
	{}
};