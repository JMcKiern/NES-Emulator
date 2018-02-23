#include <iostream>
#include <chrono>
#include "ComputerSystem.h"

int main() {
	for (int i = 0; i < 3; i++) {
		ComputerSystem computerSystem(true);
		if (i == 0) {
			computerSystem.LoadFunctionalTest("Tests/6502_functional_test.bin", 0x400, 0x336b, false);
		}
		else if (i == 1) {
			computerSystem.LoadFunctionalTest("Tests/6502_interrupt_test.bin", 0x400, 0x06f5, true);
		}
		else {
			computerSystem.LoadFunctionalTest("Tests/timingtest-1.bin", 0x1000, 0x1000, false, 0x1000);
		}
		auto startTimer = std::chrono::system_clock::now();
		int SystemReturnValue = computerSystem.Run();
	}
	
	#ifdef _DEBUG_MODE
		cpu.PrintDebugInfo();
	#endif

	char x;
	std::cin >> x;
}