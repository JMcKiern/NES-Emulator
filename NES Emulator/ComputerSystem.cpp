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
	UpdatePeripherals();
	lastPC = cpu.GetPC() - 1;
	
	auto startTimer = std::chrono::system_clock::now();
	while (lastPC != cpu.GetPC()) {
		lastPC = cpu.GetPC();
		cpu.RunNextOpcode();
#ifdef _DEBUG_MODE
		cpu.PrintDebugInfo();
#endif
		UpdatePeripherals();
	}
	auto finishTimer = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = finishTimer - startTimer;
	std::cout << "Has taken " << cpu.GetTotalCycles() << " cycles\n";
	std::cout << "          " << elapsed_seconds.count() << " second\n";
	std::cout << "\n";
	std::cout << "That's " << cpu.GetTotalCycles() / (1.0 * elapsed_seconds.count())
		<< " Hz\n";
	std::cout << '\n';
	std::cout << "Success PC: " << successPC << '\n';
	std::cout << "Final PC: " << cpu.GetPC() << '\n';

	if (hasSuccessPC) {
		if (lastPC == successPC) {
			return 0;
		}
		else {
			return -1;
		}
	}
	return -2;
}
void ComputerSystem::LoadFunctionalTest(std::string filename, uint16_t _successPC, bool shouldSetupRegisterInterrupts) {
	cpu.LoadFromFile(filename, 0);
	cpu.SetPC(0x400);
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