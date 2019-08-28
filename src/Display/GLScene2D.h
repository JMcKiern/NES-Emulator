#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "maths_funcs.h"
#include "GLUtils.h"
#include "GLEntityHandler.h"

// GLScene2D - allows an array of RGB pixels to be displayed
class GLScene2D {
private:
	const std::string vertexShaderCode =
		#include "GLScene2D_vs.glsl"
		;
	const std::string fragmentShaderCode =
		#include "GLScene2D_fs.glsl"
		;

	GLuint shader_programme;

	bool shouldScreenUpdate = true;
	int RES_X, RES_Y;
	uint8_t* pixels; // RGB channels

	GLEntityHandler glhScreen;

	void SetupPixels();

public:
	bool InitGL();
	void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
	bool DrawGLScene(GLFWwindow* window, int w_width, int w_height);
	std::string GetScreenHash();

	GLScene2D(int _RES_X, int _RES_Y);
	~GLScene2D();
};
