#pragma once
#include "Console.h"

bool Test(std::string filename, int numInstrs, std::string desiredHash) {
	Console console;
	console.LoadINES(filename);
	console.RunInstrs(numInstrs);
	std::string actualHash = console.GetFrameHash();

	bool success = actualHash == desiredHash;

	if (success) {
		std::cout << "Passed " << filename << '\n';
	}
	else {
		std::cout << "ERROR! Failed " << filename << '\n';
		char x;
		std::cin >> x;
	}
	return success;
}
void PrintHash(std::string filename) {
	std::cout << "Printing hashes for " << filename << '\n';
	Console console;
	console.LoadINES(filename);
	console.PrintHash();
}