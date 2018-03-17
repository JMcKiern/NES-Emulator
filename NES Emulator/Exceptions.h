#pragma once

#include <exception>

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
	const char* what() const throw () {
		return "Mapper has not been implemented!";
	}
};
struct InvalidFileTypeException : public std::exception {
	const char* what() const throw () {
		return "Mapper has not been implemented!";
	}
};