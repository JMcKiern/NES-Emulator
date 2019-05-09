#pragma once

#include <exception>
#include <string>

struct ItemNotFoundException : public std::exception {
	const char* what() const throw () {
		return "Item not found!";
	}
};
struct AddressAlreadyMirroredException : public std::exception {
	const char* what() const throw () {
		return "Address already mirrored!";
	}
};
struct BadInstrTimingException : public std::exception {
	const char* what() const throw () {
		return "Instruction has taken wrong number of clock cycles";
	}
};
struct MapperNotImplementedException : public std::exception {
	int mapperNum;
	const char* what() const throw () {
		return ("Mapper " + std::to_string(mapperNum) + " has not been implemented!").c_str();
	}
	MapperNotImplementedException(int _mapperNum) {
		std::exception();
		mapperNum = _mapperNum;
	}
};
struct InvalidFileTypeException : public std::exception {
	const char* what() const throw () {
		return "Invalid File Type!";
	}
};
struct MemoryAddressNotValidException : public std::exception {
	const char* what() const throw () {
		return "Memory address not valid! Either illegal or not implemented.";
	}
};