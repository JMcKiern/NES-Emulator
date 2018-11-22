#pragma once
#include "Mapper.h"

class NROM : public Mapper {
public:
	// NROM does nothing special
	NROM(std::ifstream& f);
};
