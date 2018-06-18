#pragma once

#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "maths_funcs.h"
#include "GLUtils.h"
#include "../Log.h"
#include "Camera.h"
#include "GLEntityHandler.h"

// GLScene2D - allows an array of RGB pixels to be displayed
class GLScene2D {
private:
	const std::string vertexShaderFile = "GLScene2D_vs.glsl";
	const std::string fragmentShaderFile = "GLScene2D_fs.glsl";

	GLuint shader_programme;
	GLuint vao;
	GLuint tex;
	GLuint tex1;
	mat4 projMat;
	int view_mat_location;
	int proj_mat_location;

	bool shouldScreenUpdate = true;
	int RES_X, RES_Y;
	uint8_t* pixels; // RGB channels

	GLEntityHandler glhScreen;

	Log* log;
	void SetupPixels();

public:
	bool InitGL();
	void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
	bool DrawGLScene(GLFWwindow* window, int w_width, int w_height);

	GLScene2D(Log* log, int _RES_X, int _RES_Y);
	~GLScene2D();
};