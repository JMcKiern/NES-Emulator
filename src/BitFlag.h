#pragma once

#include <cstdint>

template <uint8_t bitNum>
class BitFlag {
	uint8_t data;
public:
	void operator=(const uint8_t _data) {
		data = (_data & 0x1) ? data | (0x1 << bitNum) : data & ~(0x1 << bitNum);
	}
	operator int() const { return  ((data >> bitNum) & 0x1);}
	uint8_t GetBitNum() { return bitNum; }
};