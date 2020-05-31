#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include "Console.h"
#include "MapperFactory.h"
#include "RegisterInterrupt.h"
#include "INILoader.h"

void Console::RunFrame() {
	// A frame should take 16ms (and sleep for 16 - run_time = ~9)
	//std::cout << "Sleeping for "
	//	<< std::chrono::duration_cast<
	//	       std::chrono::milliseconds>(
	//	           nextFrame - std::chrono::system_clock::now()
	//	       ).count()
	//	<< " ms\n";
	if (shouldRunAt60fps)
		std::this_thread::sleep_until(nextFrame);

	bool isOddFrame = ppu.IsOddFrame();
	while (isOddFrame == ppu.IsOddFrame()) {
		int startCycles = cpu.GetTotalCycles();
		cpu.RunNextOpcode();
		apu.AddCycles(cpu.GetTotalCycles() - startCycles);
	}

	int cyclesRun = cpu.GetTotalCycles() - prevCycles;
	prevCycles = cpu.GetTotalCycles();
	apu.RunFrame();
	nextFrame += cycles{cyclesRun};
}
void Console::Run(std::string filename, std::string inifilename/*= ""*/) {
	LoadINES(filename);

#ifndef NES_DISABLE_IO
	if (inifilename != "") {
		INILoader iniLoader;
		iniLoader.ParseINIFile(inifilename, &io);
	}
	io.Initialize();
#endif

	nextFrame = std::chrono::system_clock::now() + cycles{0};
	while (true) {
		RunFrame();

#ifndef NES_DISABLE_IO
		io.Update();
		if (io.ShouldClose()) break;
#endif

		if (desiredHash != "" && desiredHash == ppu.GetDispHash()) break;
		if (numInstrsToRun != 0
		    && numInstrsToRun < cpu.GetTotalNumInstrs()) break;
		if (shouldPrintHash) {
			std::string currHash = GetFrameHash();
			if (prevHash != currHash) {
				std::cout << cpu.GetTotalNumInstrs() << ", \"" << currHash
					<< "\"\n";
				prevHash = currHash;
			}
		}
	}
}
void Console::RunInstrs(std::string filename, int _numInstrsToRun) {
	numInstrsToRun = _numInstrsToRun;
	Run(filename);
}
bool Console::Test(std::string filename, int _numInstrsToRun,
                   std::string _desiredHash) {
	desiredHash = _desiredHash;
	numInstrsToRun = _numInstrsToRun;
	Run(filename);
	return desiredHash == ppu.GetDispHash();
}
void Console::PrintHash(std::string filename) {
	shouldPrintHash = true;
	prevHash = GetFrameHash();
	Run(filename);
}
std::string Console::GetFrameHash() {
	return ppu.GetDispHash();
}
void Console::LoadINES(std::string filename) {
	std::cout << "Loading " << filename << '\n';
	mapperPtr = MapperFactory::GetMapper(filename, &cpu);
	cpu.PowerUp();
}

Console::Console(bool _shouldRunAt60fps/*= true*/) :
	cpu(&ppu, &apu, &mapperPtr, &io),
	ppu(&cpu, &mapperPtr, &io.gls),
	apu(&cpu),
	shouldRunAt60fps(_shouldRunAt60fps)
{}
Console::~Console() {
	glfwTerminate();
}
