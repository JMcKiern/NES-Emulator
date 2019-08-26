#pragma once
#include "../../src/Console.h"

struct NesTestRun {
	std::string filename;
	int numInstrs;
	std::string desiredHash;
};

bool Test(std::string filename, int numInstrs, std::string desiredHash) {
	Console console(true);
	return console.Test(filename, numInstrs, desiredHash);
}
void PrintHash(std::string filename) {
	std::cout << "Printing hashes for " << filename << '\n';
	Console console(true);
	console.PrintHash(filename);
}