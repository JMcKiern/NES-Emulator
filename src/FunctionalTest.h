#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <iostream>
#include "CPU_6502.h"
#include "Peripheral.h"
#include "RegisterInterrupt.h"

struct FunctionalTestRun {
	std::string filename;
	uint16_t memOffset;
	uint16_t startPC;
	uint16_t successPC;
	bool shouldRegIntr;
	bool shouldPrint;
};

bool FunctionalTest(std::string filename, uint16_t memOffset, uint16_t startPC, uint16_t _successPC, bool shouldSetupRegisterInterrupts, bool shouldPrint) {
	CPU_6502 cpu;
	std::vector<std::unique_ptr<Peripheral>> peripherals;

	if (shouldPrint) std::cout << "Loading: " << filename << '\n';
	cpu.LoadFromFile(filename, memOffset);
	cpu.SetPC(startPC);
	uint16_t successPC = _successPC;
	if (shouldSetupRegisterInterrupts) {
		peripherals.push_back(std::make_unique<RegisterInterrupt>(&cpu, 0xbffc, true, 0));	// IRQ
		peripherals.push_back(std::make_unique<RegisterInterrupt>(&cpu, 0xbffc, false, 1)); // NMI
	}
	cpu.SetI();

	if (shouldPrint) std::cout << "Running..\n";
	for (std::vector<std::unique_ptr<Peripheral>>::iterator it = peripherals.begin(); it != peripherals.end(); it++) {
		(*it)->UpdatePeripheral();
	}

	uint16_t currPC = cpu.GetPC();
	uint16_t lastPC = currPC - 1;
	auto startTimer = std::chrono::system_clock::now();
	while (lastPC != currPC) {
		lastPC = currPC;
		cpu.PrintDebugInfo();
		cpu.RunNextOpcode();
		currPC = cpu.GetPC();
		if (currPC == successPC) break;

		for (std::vector<std::unique_ptr<Peripheral>>::iterator it = peripherals.begin(); it != peripherals.end(); it++) {
			(*it)->UpdatePeripheral();
		}
	}
	auto finishTimer = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = finishTimer - startTimer;

	if (shouldPrint && currPC == successPC) {
		std::cout << "Passed " << filename << '\n';
		std::cout << "Finished with: " << cpu.GetTotalCycles() << " cycles\n";
		std::cout << "               " << elapsed_seconds.count() << " second\n";
		std::cout << "               " << cpu.GetTotalCycles() / (1.0 * elapsed_seconds.count())
			<< " Hz\n";
		std::cout << "Success PC: " << successPC << '\n';
		std::cout << "Final PC: " << currPC << "\n";
		std::cout << "Success!\n\n";
	}
	else if (currPC != successPC) { //
		std::cout << "Ran " << filename << '\n';
		std::cout << "Finished with: " << cpu.GetTotalCycles() << " cycles\n";
		std::cout << "               " << elapsed_seconds.count() << " second\n";
		std::cout << "               " << cpu.GetTotalCycles() / (1.0 * elapsed_seconds.count())
			<< " Hz\n";
		std::cout << "Success PC: " << successPC << '\n';
		std::cout << "Final PC: " << currPC << "\n";
		std::cout << "Failure!\n\n";
	}
	return currPC == successPC;
}
