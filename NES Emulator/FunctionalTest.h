#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include "CPU_6502.h"
#include "Peripheral.h"
#include "RegisterInterrupt.h"

void FunctionalTest(std::string filename, uint16_t memOffset, uint16_t startPC, uint16_t _successPC, bool shouldSetupRegisterInterrupts) {
	Log log("");
	log.SetState(false); // Do not log CPU
	CPU_6502 cpu(&log);
	std::vector<std::unique_ptr<Peripheral>> peripherals;

	std::cout << "Loading: " << filename << '\n';
	cpu.LoadFromFile(filename, memOffset);
	cpu.SetPC(startPC);
	uint16_t successPC = _successPC;
	if (shouldSetupRegisterInterrupts) {
		peripherals.push_back(std::make_unique<RegisterInterrupt>(&cpu, 0xbffc, true, 0));	// IRQ
		peripherals.push_back(std::make_unique<RegisterInterrupt>(&cpu, 0xbffc, false, 1)); // NMI
	}
	cpu.SetI();

	std::cout << "Running..\n";
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
	std::cout << "Finished with: " << cpu.GetTotalCycles() << " cycles\n";
	std::cout << "               " << elapsed_seconds.count() << " second\n";
	std::cout << "               " << cpu.GetTotalCycles() / (1.0 * elapsed_seconds.count())
		<< " Hz\n";
	std::cout << "Success PC: " << successPC << '\n';
	std::cout << "Final PC: " << currPC << "\n";

	if (currPC == successPC) {
		std::cout << "Success!\n\n";
	}
	else {
		std::cout << "Failure!\n\n";
	}
}
