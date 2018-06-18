#include <memory>
#include <chrono>
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
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
int Console::Run() {
	std::cout << "Running..\n";
	UpdatePeripherals();
	lastPC = cpu.GetPC() - 1;
	
	CreateWindow();
	SetCallbacks();
	gls.InitGL();
	auto startTimer = std::chrono::system_clock::now();
	int i = 0;
	while (lastPC != cpu.GetPC() && !glfwWindowShouldClose(window)) {
		lastPC = cpu.GetPC();
		//cpu.PrintDebugInfo();
		cpu.RunNextOpcode();
		if (hasSuccessPC && cpu.GetPC() == successPC) break;

		i++;
		if (i % 100 == 0)
			glfwPollEvents();
		UpdatePeripherals();
		if (i % 10000 == 0)
			gls.DrawGLScene(window, w_width, w_height);
	}
	auto finishTimer = std::chrono::system_clock::now();
	lastPC = cpu.GetPC();

	std::chrono::duration<double> elapsed_seconds = finishTimer - startTimer;
	std::cout << "Finished with: " << cpu.GetTotalCycles() << " cycles\n";
	std::cout << "               " << elapsed_seconds.count() << " second\n";
	std::cout << "               " << cpu.GetTotalCycles() / (1.0 * elapsed_seconds.count())
		<< " Hz\n";
	std::cout << "Success PC: " << successPC << '\n';
	std::cout << "Final PC: " << cpu.GetPC() << "\n";

	if (hasSuccessPC) {
		if (lastPC == successPC) {
			std::cout << "Success!\n\n";
			return 0;
		}
		else {
			std::cout << "Failure!\n\n";
			return -1;
		}
	}
	std::cout << "Unknown!\n\n";
	return -2;
}
void Console::LoadINES(std::string filename) {
	gp.LoadINes(filename);
	cpu.PowerUp();
}

Console::Console(std::string logFile/*= ""*/) :
	log(logFile),
	cpu(&log, &ppu, &gp, &controller),
	ppu(&cpu, &gp, &gls),
	gls(&log)
{} 
Console::~Console() {
	glfwTerminate();
}