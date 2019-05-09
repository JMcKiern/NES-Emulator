#include <fstream>
#include "Exceptions.h"
#include "MapperFactory.h"
#include "NROM.h"
#include "MMC1.h"
#include "UxROM.h"
#include "CNROM.h"
#include "MMC3.h"
#include "CPU_NES.h"

Mapper* MapperFactory::GetMapper(std::string filename, CPU_NES* cpuPtr) {
	std::ifstream f;
	std::streampos sizef;

	f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
	sizef = f.tellg(); // TODO: Check if file is at least 4 bytes long

	// Check Header
	f.seekg(0, std::ios::beg);
	const int HEADER_SIZE = 16;
	char header[HEADER_SIZE];
	f.read(header, HEADER_SIZE);
	if (!(header[0] == 'N' && header[1] == 'E' && header[2] == 'S'
	    && header[3] == 0x1A)) {
		throw InvalidFileTypeException();
	}

	// Initialize appropriate mapper
	Mapper* mapper;
	int mapperNum = ((header[6] >> 4) & 0xF) + (header[7] & 0xF0);
	switch (mapperNum) {
	case 0: mapper = new NROM(f); break;
	case 1: mapper = new MMC1(f); break;
	case 2: mapper = new UxROM(f); break;
	case 3: mapper = new CNROM(f); break;
	case 4: mapper = new MMC3(f, cpuPtr); break;

	default:
		throw MapperNotImplementedException(mapperNum);
	}
	f.close();

	return mapper;
}
