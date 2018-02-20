#pragma once

#include <exception>

struct ItemNotFoundException : public std::exception {
	const char * what() const throw () {
		return "Item not found!";
	}
};
struct AddressAlreadyMirroredException : public std::exception {
	const char * what() const throw () {
		return "Address already mirrored!";
	}
};