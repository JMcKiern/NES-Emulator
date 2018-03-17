#include <iostream>
#include <exception>
#include "MirrorManager.h"
#include "Exceptions.h"

void MirrorManager::AddMirror(uint16_t baseAddr, std::vector<uint16_t> vec, uint16_t length) {
	if (baseAddr != UnMirrorAddr(baseAddr) || (baseAddr + length - 1) != UnMirrorAddr(baseAddr + length - 1)) { // If baseAddr is already in a mirror
		throw AddressAlreadyMirroredException();
	}
	for (auto it = vec.begin(); it != vec.end(); it++) {
		if (*it != UnMirrorAddr(*it) || (*it + length - 1) != UnMirrorAddr(*it + length - 1)) { // If mirror is already in a mirror
			throw AddressAlreadyMirroredException();
		}
	}
	mirrors.push_back(Mirror(baseAddr, vec, length));
}
uint16_t MirrorManager::UnMirrorAddrSingleIter(uint16_t addr) {
	for (auto it = mirrors.begin(); it != mirrors.end(); it++) {
		addr = it->UnMirrorAddr(addr);
	}
	return addr;
}
uint16_t MirrorManager::UnMirrorAddr(uint16_t addr) {
	uint16_t prevAddr = addr;
	addr = UnMirrorAddrSingleIter(prevAddr);
	while (addr != prevAddr) {
		prevAddr = addr;
		addr = UnMirrorAddrSingleIter(prevAddr);
	}
	return addr;
}
void MirrorManager::Print() {
	int i = 0;
	for (auto it = mirrors.begin(); it != mirrors.end(); it++, i++) {
		std::cout << "Mirror " << i << ": ";
		it->Print();
		std::cout << '\n';
	}
	std::cout << '\n';
}