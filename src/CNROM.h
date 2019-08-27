#pragma once

#include <fstream>
#include "Mapper.h"

class CNROM :public Mapper {
public:
	void Write(uint16_t addr, uint8_t data) override;
	CNROM(std::ifstream& f);
};