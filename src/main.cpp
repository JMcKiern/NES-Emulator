#include <iostream>
#ifdef _WIN32
#include "Windows.h"
#endif
#include "Console.h"
#include "Logger.h"

Logger logger("log.txt");

#ifdef _WIN32
int main(int argc, char* argv[]);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd) {
	return main(__argc, __argv);
}
#endif

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
	return -1;
}
