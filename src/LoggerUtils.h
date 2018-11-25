#pragma once

#include <sstream>
#include <iomanip>

namespace LoggerUtils {
	std::string ToHexStr(uint8_t byte) {
		std::stringstream ss;
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2)
			<< (unsigned int)byte;
		return ss.str();
	}
	std::string ToHexStr(uint16_t byte) {
		std::stringstream ss;
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(4)
			<< (unsigned int)byte;
		return ss.str();
	}
}