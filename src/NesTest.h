#pragma once
#include "Console.h"

void Test(std::string filename, int numInstrs, std::string desiredHash) {
	Console console;
	console.Test(filename, desiredHash);
}
void PrintHash(std::string filename) {
	std::cout << "Printing hashes for " << filename << '\n';
	Console console;
	console.PrintHash(filename);
}