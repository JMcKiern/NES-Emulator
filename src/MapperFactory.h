#pragma once

#include <cstdint>
#include <string>
#include "Mapper.h"
#include "CPU_NES.h"

namespace MapperFactory {
	Mapper* GetMapper(std::string filename, CPU_NES* cpuPtr);
} // MapperFactory