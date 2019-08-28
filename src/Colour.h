#pragma once

#include <cstdint>

struct Colour {
	uint8_t red, green, blue;

	Colour& operator=(const Colour& other) {
		red = other.red;
		green = other.green;
		blue = other.blue;
		return *this;
	}
	Colour() {}
	Colour(const Colour& other){
		red = other.red;
		green = other.green;
		blue = other.blue;
	}
	Colour(uint8_t _r, uint8_t _g, uint8_t _b) :
		red(_r),
		green(_g),
		blue(_b)
	{}
};