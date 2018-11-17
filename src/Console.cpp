#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "Console.h"
#include "RegisterInterrupt.h"

void Console::CreateWindow() {
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
void Console::SetCallbacks() {
	glfwSetWindowUserPointer(window, this);

	auto sizeCB = [](GLFWwindow* w, int a, int b) {
		static_cast<Console*>(glfwGetWindowUserPointer(w))->
			ResizeWinCB(w, a, b);
	};
	glfwSetWindowSizeCallback(window, sizeCB);

	auto keyCB = [](GLFWwindow* w, int a, int b, int c, int d) {
		static_cast<Console*>(glfwGetWindowUserPointer(w))->
			KeyCB(w, a, b, c, d);
	};
	glfwSetKeyCallback(window, keyCB);
}
void Console::ResizeWinCB(GLFWwindow* _window, int w, int h) {
	if (window != _window) {
		throw std::runtime_error("Incorrect window");
	}
	w_width = w;
	w_height = h;
}
void Console::KeyCB(GLFWwindow* _window, int key, int scancode, int action,
                    int mods) {
	if (window != _window) return;
	controller.SetKey(keyMap[key], action); 
}
void Console::UpdatePeripherals() {
	for (std::vector<std::unique_ptr<Peripheral>>::iterator it
	         = peripherals.begin();
	     it != peripherals.end();
	     it++) {
		(*it)->UpdatePeripheral();
	}
}
void Console::CheckJoystick() {
	int count;
	const unsigned char* buttons = glfwGetJoystickButtons(joynum, &count);
	if (buttons != NULL) {
		for (int i = 0; i < 8; i++) {
			int action = buttons[gamePadKeyMap[i]];
			controller.SetJoyKey(i, action); 
		}
	}
}
void Console::RunFrame() {
	// A frame should take 16ms (and sleep for 16 - run_time = ~9)
	//std::cout << "Sleeping for "
	//	<< std::chrono::duration_cast<
	//	       std::chrono::milliseconds>(
	//	           nextFrame - std::chrono::system_clock::now()
	//	       ).count()
	//	<< " ms\n";
	std::this_thread::sleep_until(nextFrame);

	bool isOddFrame = ppu.IsOddFrame();
	while (isOddFrame == ppu.IsOddFrame()) {
		cpu.RunNextOpcode();
	}

	int cyclesRun = cpu.GetTotalCycles() - prevCycles;
	prevCycles = cpu.GetTotalCycles();
	nextFrame += cycles{cyclesRun};
}
void Console::Run(std::string filename) {
	LoadINES(filename);
	UpdatePeripherals(); // Only needed for Register Interrupts
	CreateWindow();
	SetCallbacks();
	gls.InitGL();

	nextFrame = std::chrono::system_clock::now() + cycles{0};
	while (!glfwWindowShouldClose(window)) {
		RunFrame();
		glfwPollEvents();
		if (usingGamePad) CheckJoystick();
		UpdatePeripherals(); // Only needed for Register Interrupts
		gls.DrawGLScene(window, w_width, w_height);

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
	log.SetState(false);
	numInstrsToRun = _numInstrsToRun;
	Run(filename);
}
void Console::Test(std::string filename, std::string _desiredHash) {
	log.SetState(false);
	desiredHash = _desiredHash;
	Run(filename);
}
void Console::PrintHash(std::string filename) {
	log.SetState(false);
	shouldPrintHash = true;
	std::string prevHash = GetFrameHash();
	Run(filename);
}
std::string Console::GetFrameHash() {
	return ppu.GetDispHash();
}
void Console::LoadINES(std::string filename) {
	std::cout << "Loading " << filename << '\n';
	gp.LoadINes(filename);
	cpu.PowerUp();
}

Console::Console(std::string logFile/*= ""*/) :
	log(logFile),
	cpu(&log, &ppu, &gp, &controller),
	ppu(&cpu, &gp, &gls),
	gls(&log, RES_X, RES_Y)
{} 
Console::~Console() {
	glfwTerminate();
}
