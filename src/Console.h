#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <iostream>
#include <stdint.h>
#include "CPU_NES.h"
#include "Controller.h"
#include "Display/GLScene2D.h"
#include "Log.h"
#include "Peripheral.h"

// Contains everything to do with everything
// i.e. CPU, PPU, APU, Peripherals, Screen 
class Console {
	CPU_NES cpu;
	PPU ppu;
	GamePak gp;

	Log log;

	// Window
	GLFWwindow* window;
	const std::string windowTitle = "Window Title";
	int w_width = 512, w_height = 480;
	void CreateWindow();
	void SetCallbacks();
	void ResizeWinCB(GLFWwindow* _window, int w, int h);
	void KeyCB(GLFWwindow* _window, int key, int scancode, int action,
	           int mods);

	// OpenGL
	const int RES_X = 256;
	const int RES_Y = 240;
	GLScene2D gls;

	// Peripherals
	Controller controller;
	std::vector<std::unique_ptr<Peripheral>> peripherals;
	void UpdatePeripherals();
	std::map<int, int> keyMap = {
		{GLFW_KEY_J, 0}, // A
		{GLFW_KEY_K, 1}, // B
		{GLFW_KEY_G, 2}, // Select
		{GLFW_KEY_H, 3}, // Start
		{GLFW_KEY_W, 4}, // Up
		{GLFW_KEY_S, 5}, // Down
		{GLFW_KEY_A, 6}, // Left
		{GLFW_KEY_D, 7}  // Right
	};
	int joynum = GLFW_JOYSTICK_1;
	void CheckJoystick();
	bool usingGamePad = true;
	int gamePadKeyMap[8] = {
		0,  // A
		1,  // B
		6,  // Select
		7,  // Start
		10, // Up
		12, // Down
		13, // Left
		11  // Right
	};

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

public:
	void RunFrame();
	void Run(std::string filename);
	void RunInstrs(std::string filename, int _numInstrs);
	void Test(std::string filename, std::string _desiredHash);
	void PrintHash(std::string filename);
	std::string GetFrameHash();
	void LoadINES(std::string filename);

	Console(std::string logFile = "");
	~Console();
};
