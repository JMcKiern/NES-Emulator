#include "Mirror.h"

bool Mirror::ContainsAddress(uint16_t addr) {
	for (auto it = mirrorBaseAddresses.begin(); it != mirrorBaseAddresses.end(); it++) {
		if (addr >= *it && ((addr - *it) < length)) {
			return true;
		}
	}
	return false;
}
bool Mirror::ContainsAddress(uint16_t addr, uint16_t& offsetFromMirror) {
	for (auto it = mirrorBaseAddresses.begin(); it != mirrorBaseAddresses.end(); it++) {
		if (addr >= *it && ((addr - *it) < length)) {
			offsetFromMirror = addr - *it;
			return true;
		}
	}
	return false;
}
uint16_t Mirror::GetBaseAddr() {
	return baseAddr;
}
uint16_t Mirror::UnMirrorAddr(uint16_t addr) {
	uint16_t offset;
	if (ContainsAddress(addr, offset)) {
		return baseAddr + offset;
	}
	else {
		return addr;
	}
}
std::vector<uint16_t> Mirror::GetVec() {
	return mirrorBaseAddresses;
}
uint16_t Mirror::GetLength() {
	return length;
}
void Mirror::Print() {
	for (auto it = mirrorBaseAddresses.begin(); it != mirrorBaseAddresses.end(); it++) {
		std::cout << *it << ", ";
	}
}
Mirror Mirror::operator=(const Mirror& other) {
	mirrorBaseAddresses = other.mirrorBaseAddresses;
	length = other.length;
	return *this;
}
//Mirror::Mirror() {
//	length = 0;
//}
Mirror::Mirror(uint16_t _baseAddr, std::vector<uint16_t> vec, uint16_t _length) {
	baseAddr = _baseAddr;
	mirrorBaseAddresses = vec;
	length = _length;
}