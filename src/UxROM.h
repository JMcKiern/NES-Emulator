#pragma once

#include <fstream>
#include "Mapper.h"

class UxROM : public Mapper {
public:
	void Write(uint16_t addr, uint8_t data);
	UxROM(std::ifstream& f);
};