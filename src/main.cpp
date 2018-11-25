#include <iostream>
#include "Console.h"
#include "Logger.h"

Logger logger("log.txt");

int main(int argc, char* argv[]) {
	if (argc == 2) {
		Console console;
		console.Run(argv[1]);
		return 0;
	}
	else if (argc < 2) {
		std::cout << "You must supply a file to run!\n";
	}
	else {
		std::cout << "You cannot supply more than one file!\n";
	}
	char x;
	std::cin >> x;
}
