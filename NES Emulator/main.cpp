#include <iostream>
#include <chrono>
#include "ComputerSystem.h"

int main() {
	for (int i = 0; i < 2; i++) {
		ComputerSystem computerSystem(true);
		if (i == 0) {
			computerSystem.LoadFunctionalTest("Tests/6502_functional_test.bin", 0x336b, false);
		}
		else {
			computerSystem.LoadFunctionalTest("Tests/6502_interrupt_test.bin", 0x06f5, true);
		}
		auto startTimer = std::chrono::system_clock::now();
		int SystemReturnValue = computerSystem.Run();

		if (SystemReturnValue == 0) {
			std::cout << "SUCCESS!\n";
		}
		else {
			std::cout << "Computer System has returned: " << SystemReturnValue << '\n';
		}

	}
	
	#ifdef _DEBUG_MODE
		cpu.PrintDebugInfo();
	#endif

	char x;
	std::cin >> x;
}