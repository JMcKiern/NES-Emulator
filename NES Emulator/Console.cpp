#include <memory>
#include <chrono>
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <thread>
#include "Console.h"
#include "RegisterInterrupt.h"

void Console::UpdatePeripherals() {
	for (std::vector<std::unique_ptr<Peripheral>>::iterator it = peripherals.begin(); it != peripherals.end(); it++) {
		(*it)->UpdatePeripheral();
	}
}
void Console::CreateWindow() {
	if (!glfwInit()) {
		throw std::exception("Error: GLFW failed to initialise");
	}

	window = glfwCreateWindow(w_width, w_height, windowTitle.c_str(), NULL, NULL);
	if (!window) {
		throw std::exception("Error: GLFW failed to create window");
	}
	glfwMakeContextCurrent(window);
}
void Console::SetCallbacks() {
	glfwSetWindowUserPointer(window, this);
	auto sizeCB = [](GLFWwindow* w, int a, int b) {
		static_cast<Console*>(glfwGetWindowUserPointer(w))->ResizeWinCB(w, a, b);
	};
	glfwSetWindowSizeCallback(window, sizeCB);
	auto keyCB = [](GLFWwindow* w, int a, int b, int c, int d) {
		static_cast<Console*>(glfwGetWindowUserPointer(w))->KeyCB(w, a, b, c, d);
	};
	glfwSetKeyCallback(window, keyCB);
}
void Console::ResizeWinCB(GLFWwindow* _window, int w, int h) {
	if (window != _window) {
		throw std::exception("Incorrect window");
	}
	w_width = w;
	w_height = h;
}
void Console::KeyCB(GLFWwindow* _window, int key, int scancode, int action, int mods) {
	if (window != _window) return;
	for (int i = 0; i < 8; i++) {
		if (key == keyMap[i]) {
			//controller.SetKey(i, action); 
			if (action == GLFW_PRESS) {
				controller.SetKey(i, true);
			}
			else if (action == GLFW_RELEASE) {
				controller.SetKey(i, false);
			}
			break;
		}
	}
}
void Console::RunFrame() {
	using cycles = std::chrono::duration<int64_t, std::ratio<1, CLOCK_SPEED>>;
	static int prevCycles = cpu.GetTotalCycles();
	static auto nextFrame = std::chrono::system_clock::now() + cycles{ 0 };
	std::this_thread::sleep_until(nextFrame);

	bool isOddFrame = ppu.IsOddFrame();
	while (isOddFrame == ppu.IsOddFrame()) {
		cpu.RunNextOpcode();
	}

	int cyclesRun = cpu.GetTotalCycles() - prevCycles;
	prevCycles = cpu.GetTotalCycles();
	nextFrame += cycles{ cyclesRun };
}
void Console::Run() {
	UpdatePeripherals(); // Only needed for Register Interrupts
	CreateWindow();
	SetCallbacks();
	gls.InitGL();

	while (!glfwWindowShouldClose(window)) {
		RunFrame();
		glfwPollEvents();
		UpdatePeripherals(); // Only needed for Register Interrupts
		gls.DrawGLScene(window, w_width, w_height);
	}
}
void Console::RunInstrs(int numInstrsToRun) {
	log.SetState(false);
	UpdatePeripherals(); // Only needed for Register Interrupts
	
	CreateWindow();
	SetCallbacks();
	gls.InitGL();

	int currInstrNum = 0;
	while (!glfwWindowShouldClose(window) && currInstrNum <= numInstrsToRun) {
		RunFrame();
		glfwPollEvents();
		UpdatePeripherals(); // Only needed for Register Interrupts
		gls.DrawGLScene(window, w_width, w_height);
		currInstrNum = cpu.GetTotalNumInstrs();
	}
}
void Console::PrintHash() {
	log.SetState(false);
	UpdatePeripherals(); // Only needed for Register Interrupts
	
	CreateWindow();
	SetCallbacks();
	gls.InitGL();

	std::string prevHash = GetFrameHash();
	while (!glfwWindowShouldClose(window)) {
		RunFrame();
		glfwPollEvents();
		UpdatePeripherals(); // Only needed for Register Interrupts
		gls.DrawGLScene(window, w_width, w_height);

		std::string currHash = GetFrameHash();
		if (prevHash != currHash) {
			std::cout << "\"" << currHash << "\"\n";
			prevHash = currHash;
		}
	}
}
std::string Console::GetFrameHash() {
	return gls.GetScreenHash();
}
void Console::LoadINES(std::string filename) {
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