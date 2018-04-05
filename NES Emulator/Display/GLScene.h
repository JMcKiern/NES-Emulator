#pragma once

#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "maths_funcs.h"
#include "GLUtils.h"
#include "../Log.h"
#include "Camera.h"
#include "GLEntityHandler.h"

// Deals with everything to do with the OpenGL scene including aw, ic and cam
// Leaves the OpenGL implementation to
// TODO:
//		Move general functions to GLUtils? (eg readShaderFile(), load_texture(), loadShaderProg())
class GLScene {
private:
	const double scaleFactor = 1;

	const std::string vertexShaderFile = "vearrt.txt";
	const std::string fragmentShaderFile = "frage.txt";// "C:\\Users\\Jack\\source\\repos\\NES Emulator\\NES Emulator\\Display\\test.frag";
	std::string readShaderFile(std::string fileName);
	GLuint loadShaderProg(std::string vertexShaderFile, std::string fragmentShaderFile);

	GLuint shader_programme;
	GLuint vao;
	GLuint tex;
	GLuint tex1;
	mat4 projMat;
	int view_mat_location;
	int proj_mat_location;

	bool shouldScreenUpdate = true;
	static const int RES_X = 256;
	static const int RES_Y = 240;
	uint8_t pixels[3 * RES_X * RES_Y]; // RGB channels
	unsigned char* load_texture(const char *file_name, int& _width, int& _height);

	GLEntityHandler glhScreen;

	Log* log;

public:

	void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
	bool InitGL();
	bool DrawGLScene(GLFWwindow* window, int w_width, int w_height);

	GLScene(Log* log);
};