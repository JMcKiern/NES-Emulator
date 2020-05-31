#pragma once

#include <map>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Controller.h"
#include "Display/GLScene2D.h"
#include "Peripheral.h"

class IO {
private:
	// Peripherals
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
	int framesSinceNewFPS = 0;
	void UpdateFPSCounter(int numFrames=1);
	bool isFullscreen = false;
	int old_xpos, old_ypos, old_width, old_height;
	void ToggleFullscreen();

	bool isInitialized = false;

public:
	Controller controller0, controller1;

	// OpenGL
	const int RES_X = 256;
	const int RES_Y = 240;
	GLScene2D gls = GLScene2D(RES_X, RES_Y);

	void Initialize();
	void SetContollerKeyMap(uint8_t ctrlNum, std::map<int, int> keymap);
	void Update(int framesRun = 1);
	bool ShouldClose();
};
