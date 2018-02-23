#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include "CPU.h"
#include "Peripheral.h"

const std::string logFile = "log.txt";

// Contains everything to do with everything
// i.e. CPU, PPU, APU, Peripherals, Screen 
class ComputerSystem {
	CPU cpu;
	std::vector<std::unique_ptr<Peripheral>> peripherals;
	void UpdatePeripherals();

	// Used for error checking
	uint16_t lastPC; // Used to detect loop on single instruction
	bool hasSuccessPC;
	uint16_t successPC;
public:
	int Run();
	void LoadFunctionalTest(std::string filename, uint16_t startPC, uint16_t successPC, bool shouldSetupRegisterInterrupts, uint16_t memOffset = 0x0);
	ComputerSystem();
	ComputerSystem(bool shouldSetupBlank);
};

