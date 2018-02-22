#include <iostream>
#include <fstream>
#include "CPU.h"
#include "MirrorManager.h"

//uint8_t test[] = {0xa9, 0x01, 0x8d, 0x00, 0x02, 0xa9, 0x05, 0x8d ,0x01, 0x02, 0xa9, 0x08, 0x8d, 0x02, 0x02};
//uint8_t test[] = {0xa9, 0xc0, 0xaa, 0xe8, 0x69, 0xc4, 0x00}; // BRK doesn't really work
//uint8_t test[] = {0xa2, 0x08, 0xca, 0x8e, 0x00, 0x02, 0xe0, 0x03, 0xd0, 0xf8, 0x8e, 0x01, 0x02, 0x00};
//uint8_t test[] = {0x20, 0x09, 0x06, 0x20, 0x0c, 0x06, 0x20, 0x12, 0x06, 0xa2, 0x00, 0x60, 0xe8, 0xe0, 0x05, 0xd0, 
//0xfb, 0x60, 0x00}; const uint8_t len = 19;
uint8_t test[] = { 0x38, 0xa9, 0xfb, 0x69, 0x01, 0x38, 0xa9, 0xff, 0x69, 0x01, 0x18, 0xa9, 0xff, 0x69, 0x01 };
const uint8_t len = 15;

const std::string logFile = "log.txt";

int main() {
	CPU cpu(false, logFile);
	//CPU cpu;
	std::cout << "Loaded CPU\n";
	//cpu.LOADTEST(test, len);
	//cpu.C64TestStartup("C:\\Users\\Jack\\Downloads\\testsuite - 2.15.tar\\testsuite - 2.15\\bin\\start");
	//cpu.FunctionalTestStartup("Tests/6502_functional_test.bin");
	cpu.FunctionalTestStartup("Tests/6502_interrupt_test.bin");
	std::cout << "Loaded test\n";


	char x;
	#ifdef _DEBUG_MODE
		cpu.PrintDebugInfo();
	#endif	
	uint16_t PC = cpu.GetPC();
	int i = 0;
	while (true) {
 		cpu.RunNextOpcode();
		//if (cpu.GetPC() == 0x336d) break; // Success
		if (cpu.GetPC() == 0x06f5) break; // Success
		if (PC == cpu.GetPC()) throw -1;
		PC = cpu.GetPC();
		#ifdef _DEBUG_MODE
			cpu.PrintDebugInfo();
			//std::cin >> x;
		#endif
	}
	std::cout << "SUCCESS!\n";
	#ifdef _DEBUG_MODE
		cpu.PrintDebugInfo();
	#endif

	std::cin >> x;
}