#pragma once

#include <vector>
#include <cstdint>
#include <iostream>

class Mirror {
private:
	uint16_t baseAddr;
	std::vector<uint16_t> mirrorBaseAddresses;
	uint16_t length;
public:
	bool ContainsAddress(uint16_t addr);
	bool ContainsAddress(uint16_t addr, uint16_t& baseAddr);
	uint16_t GetBaseAddr();
	uint16_t UnMirrorAddr(uint16_t addr);
	std::vector<uint16_t> GetVec();
	uint16_t GetLength();
	void Print();

	Mirror operator=(const Mirror& other);

	//Mirror();
	Mirror(uint16_t baseAddr, std::vector<uint16_t> vec, uint16_t _length);
};

//class Mirror {
//private:
//	std::vector<uint16_t> addresses;
//	uint16_t length;
//public:
//	bool ContainsAddress(uint16_t addr);
//	bool ContainsAddress(uint16_t addr, uint16_t& offsetFromMirror);
//	std::vector<uint16_t> GetVec();
//	uint16_t GetLength();
//	void Print();
//
//	Mirror operator=(const Mirror& other);
//
//	Mirror();
//	Mirror(std::vector<uint16_t> vec, uint16_t _length);
//};