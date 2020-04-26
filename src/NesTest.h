#pragma once
#include "Console.h"

struct NesTestRun {
	std::string filename;
	int numInstrs;
	std::string desiredHash;
};

bool Test(std::string filename, int numInstrs, std::string desiredHash) {
	Console console(false);
	return console.Test(filename, numInstrs, desiredHash);
}
void PrintHash(std::string filename) {
	std::cout << "Printing hashes for " << filename << '\n';
	Console console(false);
	console.PrintHash(filename);
}