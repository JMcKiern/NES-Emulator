#include <memory>
#include <chrono>
#include <iostream>
#include "ComputerSystem.h"
#include "RegisterInterrupt.h"

void ComputerSystem::UpdatePeripherals() {
	for (std::vector<std::unique_ptr<Peripheral>>::iterator it = peripherals.begin(); it != peripherals.end(); it++) {
		(*it)->UpdatePeripheral();
	}
}
int ComputerSystem::Run() {
	std::cout << "Running..\n";
	UpdatePeripherals();
	lastPC = cpu.GetPC() - 1;
	
	auto startTimer = std::chrono::system_clock::now();
	while (lastPC != cpu.GetPC()) {
		lastPC = cpu.GetPC();
		cpu.RunNextOpcode();
		if (hasSuccessPC && cpu.GetPC() == successPC) break;
#ifdef _DEBUG_MODE
		cpu.PrintDebugInfo();
#endif
		UpdatePeripherals();
	}
	auto finishTimer = std::chrono::system_clock::now();
	lastPC = cpu.GetPC();

	std::chrono::duration<double> elapsed_seconds = finishTimer - startTimer;
	std::cout << "Finished with: " << cpu.GetTotalCycles() << " cycles\n";
	std::cout << "               " << elapsed_seconds.count() << " second\n";
	std::cout << "               " << cpu.GetTotalCycles() / (1.0 * elapsed_seconds.count())
		<< " Hz\n";
	std::cout << "Success PC: " << successPC << '\n';
	std::cout << "Final PC: " << cpu.GetPC() << "\n";

	if (hasSuccessPC) {
		if (lastPC == successPC) {
			std::cout << "Success!\n\n";
			return 0;
		}
		else {
			std::cout << "Failure!\n\n";
			return -1;
		}
	}
	std::cout << "Unknown!\n\n";
	return -2;
}
void ComputerSystem::LoadFunctionalTest(std::string filename, uint16_t startPC, uint16_t _successPC, bool shouldSetupRegisterInterrupts, uint16_t memOffset) {
	std::cout << "Loading: " << filename << '\n';
	cpu.LoadFromFile(filename, memOffset);
	cpu.SetPC(startPC);
	successPC = _successPC;
	hasSuccessPC = true;
	if (shouldSetupRegisterInterrupts) {
		peripherals.push_back(std::make_unique<RegisterInterrupt>(&cpu, 0xbffc, true, 0));	// IRQ
		peripherals.push_back(std::make_unique<RegisterInterrupt>(&cpu, 0xbffc, false, 1)); // NMI
	}
	cpu.SetI();
}

ComputerSystem::ComputerSystem() {

}
ComputerSystem::ComputerSystem(bool shouldSetupBlank) :
	cpu(true)
{
}