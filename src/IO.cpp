#include <chrono>
#include <stdexcept>
#include "IO.h"

void IO::InitializeWindow() {
	if (!glfwInit()) {
		throw std::runtime_error("Error: GLFW failed to initialise");
	}

	window = glfwCreateWindow(w_width, w_height, windowTitle.c_str(), NULL,
	                          NULL);
	if (!window) {
		throw std::runtime_error("Error: GLFW failed to create window");
	}
	glfwMakeContextCurrent(window);
}
void IO::SetCallbacks() {
	glfwSetWindowUserPointer(window, this);

	auto sizeCB = [](GLFWwindow* w, int a, int b) {
		static_cast<IO*>(glfwGetWindowUserPointer(w))->
			ResizeWinCB(w, a, b);
	};
	glfwSetWindowSizeCallback(window, sizeCB);

	auto keyCB = [](GLFWwindow* w, int a, int b, int c, int d) {
		static_cast<IO*>(glfwGetWindowUserPointer(w))->
			KeyCB(w, a, b, c, d);
	};
	glfwSetKeyCallback(window, keyCB);
}
void IO::ResizeWinCB(GLFWwindow* _window, int w, int h) {
	if (window != _window) {
		throw std::runtime_error("Incorrect window");
	}
	w_width = w;
	w_height = h;
}
void IO::KeyCB(GLFWwindow* _window, int key, int scancode, int action,
                    int mods) {
	if (window != _window) return;
	if (controller0KeyMap.count(key) > 0)
		controller0.SetKey(controller0KeyMap[key], action);
	if (controller1KeyMap.count(key) > 0)
		controller1.SetKey(controller1KeyMap[key], action);
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) ToggleFullscreen();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
}
void IO::UpdatePeripherals() {
	for (std::vector<std::unique_ptr<Peripheral>>::iterator it
	         = peripherals.begin();
	     it != peripherals.end();
	     it++) {
		(*it)->UpdatePeripheral();
	}
}
void IO::CheckJoystick() {
	int count;
	const unsigned char* buttons = glfwGetJoystickButtons(joynum, &count);
	if (buttons != NULL) {
		for (int i = 0; i < 8; i++) {
			int action = buttons[gamePadKeyMap[i]];
			controller0.SetJoyKey(i, action);
		}
	}
}
void IO::UpdateFPSCounter(int numFrames/*=1*/) {
	static auto prevTime = std::chrono::system_clock::now();
	auto dT = std::chrono::duration_cast<
		std::chrono::milliseconds>(
			std::chrono::system_clock::now() - prevTime
		).count();
	if (dT == 0) {
		dT = 1;
	}

	// Truncates the value but that's fine
	std::string title = "FPS: " + std::to_string(numFrames * 1000 / dT);
	glfwSetWindowTitle(window, title.c_str());
	prevTime = std::chrono::system_clock::now();
}
void IO::ToggleFullscreen() {
	if (isFullscreen) {
		// Return to windowed
		glfwSetWindowMonitor(window, NULL, old_xpos, old_ypos, old_width,
				old_height, 0);
		isFullscreen = false;
	}
	else {
		// Save window pos and size
		glfwGetWindowPos(window, &old_xpos, &old_ypos);
		old_width = w_width;
		old_height = w_height;

		// Set fullscreen
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height,
				mode->refreshRate);
		isFullscreen = true;
	}
}

void IO::Initialize() {
	UpdatePeripherals(); // Only needed for Register Interrupts
	InitializeWindow();
	SetCallbacks();
	gls.InitGL();
	isInitialized = true;
}

void IO::SetContollerKeyMap(uint8_t ctrlNum, std::map<int, int> keymap) {
	switch(ctrlNum) {
		default:
		case 0:
			controller0KeyMap = keymap;
			break;
		case 1:
			controller1KeyMap = keymap;
			break;
	}
}

void IO::Update(int framesRun/*= 1*/) {
	if (!isInitialized)
		throw std::logic_error("IO must be initialized before use");
	framesSinceNewFPS += framesRun;
	if (framesSinceNewFPS > FPS_WINDOW) {
		UpdateFPSCounter(framesSinceNewFPS);
		framesSinceNewFPS = 0;
	}
	glfwPollEvents();
	if (usingGamePad) CheckJoystick();
	UpdatePeripherals(); // Only needed for Register Interrupts
	gls.DrawGLScene(window, w_width, w_height);
}

bool IO::ShouldClose() {
	return glfwWindowShouldClose(window);
}
