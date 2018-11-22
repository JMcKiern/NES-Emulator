#pragma once

#include <cstdint>
#include <string>
#include "Mapper.h"

namespace MapperFactory {
	Mapper* GetMapper(std::string filename);
} // MapperFactory