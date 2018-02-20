#pragma once

#include <cstdint>
#include <vector>
#include "Mirror.h"

class MirrorManager {
	std::vector<Mirror> mirrors;

	uint16_t UnMirrorAddrSingleIter(uint16_t addr);
public:
	void AddMirror(uint16_t baseAddr, std::vector<uint16_t> vec, uint16_t length);
	uint16_t UnMirrorAddr(uint16_t addr);
	void Print();
};