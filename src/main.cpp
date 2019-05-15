#include <iostream>
#ifdef _WIN32
#include "Windows.h"
#endif
#include "Console.h"
#include "Logger.h"

//Logger logger("log.txt");
Logger logger();

#ifdef _WIN32
int main(int argc, char* argv[]);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd) {
	return main(__argc, __argv);
}
void displayError(std::string err) {
	MessageBox(
		NULL,
		err.c_str(),
		NULL,
		MB_OK
	);
}
#else
void displayError(std::string err) {
	std::cerr << err;
}
#endif

int main(int argc, char* argv[]) {
	try {
		if (argc == 2) {
			Console console;
			console.Run(__argv[1]);
		}
		else if (argc < 2) {
			throw std::runtime_error("You must supply a file to run!\n");
		}
		else {
			throw std::runtime_error("You cannot supply more than one file!\n");
		}
		return 0;
	}
	catch (const std::exception& e) {
		displayError(e.what());
		return -1;
	}
}
