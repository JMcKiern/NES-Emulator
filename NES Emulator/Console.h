#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include "CPU_NES.h"
#include "Peripheral.h"
#include "Log.h"
#include "Display\GLScene.h"
#include "Controller.h"

// Contains everything to do with everything
// i.e. CPU, PPU, APU, Peripherals, Screen 
class Console {
	CPU_NES cpu;
	PPU ppu;
	std::vector<std::unique_ptr<Peripheral>> peripherals;
	void UpdatePeripherals();
	GamePak gp;
	Log log;
	Controller controller;
	// TODO: Replace with map?
	int keyMap[8] = {
		GLFW_KEY_J, // A
		GLFW_KEY_K, // B
		GLFW_KEY_G, // Select
		GLFW_KEY_H, // Start
		GLFW_KEY_W, // Up
		GLFW_KEY_S, // Down
		GLFW_KEY_A, // Left
		GLFW_KEY_D  // Right
	};
	// TODO: Add gamepad support
	/*
	bool usingGamePad = true;
	int gamePadKeyMap[8] = {
		GLFW_JOYSTICK_1, // A
		GLFW_JOYSTICK_2, // B
		GLFW_JOYSTICK_7, // Select
		GLFW_JOYSTICK_8, // Start
		GLFW_JOYSTICK_10, // Up
		GLFW_JOYSTICK_12, // Down
		GLFW_JOYSTICK_13, // Left
		GLFW_JOYSTICK_11, // Right
	};*/

	// Window
	GLFWwindow* window;
	const std::string windowTitle = "Window Title";
	int w_width = 512, w_height = 480;
	void CreateWindow();
	void SetCallbacks();
	void ResizeWinCB(GLFWwindow* _window, int w, int h);
	void KeyCB(GLFWwindow* _window, int key, int scancode, int action, int mods);
	// Error checking
	uint16_t lastPC; // Used to detect loop on single instruction
	bool hasSuccessPC;
	uint16_t successPC;
	GLScene gls;
public:
	int Run();
	void LoadINES(std::string filename);
	Console(std::string logFile = "");
	~Console();
};