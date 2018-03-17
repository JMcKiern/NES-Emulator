#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include "CPU_NES.h"
#include "Peripheral.h"
#include "Log.h"

// Contains everything to do with everything
// i.e. CPU, PPU, APU, Peripherals, Screen 
class ComputerSystem {
	CPU_NES cpu;
	std::vector<std::unique_ptr<Peripheral>> peripherals;
	void UpdatePeripherals();
	GamePak gp;
	Log log;

	// Error checking
	uint16_t lastPC; // Used to detect loop on single instruction
	bool hasSuccessPC;
	uint16_t successPC;
public:
	int Run();
	void LoadINES(std::string filename, uint16_t startPC);
	ComputerSystem(std::string logFile = "");
};