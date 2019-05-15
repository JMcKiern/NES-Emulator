#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <iostream>
#include <stdint.h>
#include "CPU_NES.h"
#include "APU.h"
#include "Controller.h"
#include "Display/GLScene2D.h"
#include "Peripheral.h"

// Contains everything to do with everything
// i.e. CPU, PPU, APU, Peripherals, Screen 
class Console {
	CPU_NES cpu;
	PPU ppu;
	APU apu;
	Mapper* mapperPtr;

	// Window
	GLFWwindow* window;
	const std::string windowTitle = "Window Title";
	int w_width = 512, w_height = 480;
	void InitializeWindow();
	void SetCallbacks();
	void ResizeWinCB(GLFWwindow* _window, int w, int h);
	void KeyCB(GLFWwindow* _window, int key, int scancode, int action,
	           int mods);
	const int FPS_WINDOW = 10;
	void UpdateFPSCounter(int numFrames=1);

	// OpenGL
	const int RES_X = 256;
	const int RES_Y = 240;
	GLScene2D gls;

	// Peripherals
	Controller controller0, controller1;
	std::vector<std::unique_ptr<Peripheral>> peripherals;
	void UpdatePeripherals();
	std::map<int, int> controller0KeyMap = {
		{GLFW_KEY_K, 0}, // A
		{GLFW_KEY_J, 1}, // B
		{GLFW_KEY_G, 2}, // Select
		{GLFW_KEY_H, 3}, // Start
		{GLFW_KEY_W, 4}, // Up
		{GLFW_KEY_S, 5}, // Down
		{GLFW_KEY_A, 6}, // Left
		{GLFW_KEY_D, 7}  // Right
	};
	std::map<int, int> controller1KeyMap = {
		{GLFW_KEY_KP_3, 0}, // A
		{GLFW_KEY_KP_0, 1}, // B
		{GLFW_KEY_KP_1, 2}, // Select
		{GLFW_KEY_KP_2, 3}, // Start
		{GLFW_KEY_UP, 4}, // Up
		{GLFW_KEY_DOWN, 5}, // Down
		{GLFW_KEY_LEFT, 6}, // Left
		{GLFW_KEY_RIGHT, 7}  // Right
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
	bool Test(std::string filename, int _numInstrsToRun,
	          std::string _desiredHash);
	void PrintHash(std::string filename);
	std::string GetFrameHash();
	void LoadINES(std::string filename);

	Console();
	~Console();
};
