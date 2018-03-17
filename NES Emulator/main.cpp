#include <iostream>
#include "FunctionalTest.h"
#include "ComputerSystem.h"

int main() {
	{
		FunctionalTest("Tests/6502_functional_test.bin", 0x0, 0x400, 0x336d, false);
		FunctionalTest("Tests/6502_interrupt_test.bin", 0x0, 0x400, 0x06f5, true);
		FunctionalTest("Tests/timingtest-1.bin", 0x1000, 0x1000, 0x1000, false);
	}
	{
		const std::string logFile = "nestest";
		ComputerSystem computerSystem(logFile);
		computerSystem.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\nestest.nes", 0xC000);
		int SystemReturnValue = computerSystem.Run();
	}

	char x;
	std::cin >> x;
}