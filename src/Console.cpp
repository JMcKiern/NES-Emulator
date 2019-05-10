#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "Console.h"
#include "MapperFactory.h"
#include "RegisterInterrupt.h"

void Console::InitializeWindow() {
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
	if (controller0KeyMap.count(key) > 0)
		controller0.SetKey(controller0KeyMap[key], action); 
	if (controller1KeyMap.count(key) > 0)
		controller1.SetKey(controller1KeyMap[key], action); 
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
			controller0.SetJoyKey(i, action); 
		}
	}
}
void Console::UpdateFPSCounter(int numFrames/*=1*/) {
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
void Console::RunFrame() {
	// A frame should take 16ms (and sleep for 16 - run_time = ~9)
	//std::cout << "Sleeping for "
	//	<< std::chrono::duration_cast<
	//	       std::chrono::milliseconds>(
	//	           nextFrame - std::chrono::system_clock::now()
	//	       ).count()
	//	<< " ms\n";
	std::this_thread::sleep_until(nextFrame);

	int startFrameCycle = cpu.GetTotalCycles();

	bool isOddFrame = ppu.IsOddFrame();
	while (isOddFrame == ppu.IsOddFrame()) {
		cpu.RunNextOpcode();
	}

	int numCycles = cpu.GetTotalCycles() - startFrameCycle;
	apu.RunFrame(numCycles);


	int cyclesRun = cpu.GetTotalCycles() - prevCycles;
	prevCycles = cpu.GetTotalCycles();
	nextFrame += cycles{cyclesRun};
}
void Console::Run(std::string filename) {
	LoadINES(filename);
	UpdatePeripherals(); // Only needed for Register Interrupts
	InitializeWindow();
	SetCallbacks();
	gls.InitGL();

	int i = 0;
	nextFrame = std::chrono::system_clock::now() + cycles{0};
	while (!glfwWindowShouldClose(window)) {
		i++;
		RunFrame();
		if (i % FPS_WINDOW == 0) {
			UpdateFPSCounter(FPS_WINDOW);
			i = 0;
		}
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

Console::Console() :
	cpu(&ppu, &mapperPtr, &controller0, &controller1),
	ppu(&cpu, &mapperPtr, &gls),
	gls(RES_X, RES_Y)
{} 
Console::~Console() {
	glfwTerminate();
}
