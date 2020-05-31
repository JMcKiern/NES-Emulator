#pragma once

#include <fstream>
#include <memory>
#include <iostream>
#include <stdint.h>
#include "CPU_NES.h"
#include "APU.h"
#include "IO.h"

// Contains everything to do with everything
// i.e. CPU, PPU, APU, IO (Peripherals, Screen)
class Console {
	CPU_NES cpu;
	PPU ppu;
	APU apu;
	IO io;
	Mapper* mapperPtr;

	const static int CLOCK_SPEED = 1790000; // 1.79 MHz
	using cycles = std::chrono::duration<int64_t, std::ratio<1, CLOCK_SPEED>>;
	std::chrono::time_point<
		std::chrono::system_clock,
		std::chrono::duration<
			std::common_type_t<std::chrono::system_clock::rep,
				std::chrono::system_clock::rep
			>,
			std::ratio<INT64_C(1), INT64_C(CLOCK_SPEED * 1000000)>
		>
	> nextFrame;

	int prevCycles = cpu.GetTotalCycles();

	// Console::Run() variables
	int numInstrsToRun = 0;
	std::string desiredHash = "";
	bool shouldPrintHash = false;
	std::string prevHash;
	bool shouldRunAt60fps = true;

public:
	void RunFrame();
	void Run(std::string filename, std::string inifilename = "");
	void RunInstrs(std::string filename, int _numInstrs);
	bool Test(std::string filename, int _numInstrsToRun,
	          std::string _desiredHash);
	void PrintHash(std::string filename);
	std::string GetFrameHash();
	void LoadINES(std::string filename);

	Console(bool _shouldRunAt60fps = true);
	~Console();
};
