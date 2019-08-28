#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "GLScene2D.h"

void GLScene2D::SetupPixels() {
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			uint8_t r, g, b;
			bool isGrey = y % 2 == 0 ? x % 2 == 0 : (x - 1) % 2 == 0;
			if (isGrey) {
				r = g = b = 0xFF;
			}
			else {
				r = g = b = 0xAA;
			}
			SetPixel(x, y, r, g, b);
		}
	}
}

bool GLScene2D::InitGL() {
	// Setup OpenGL
	glewExperimental = GL_TRUE;
	glewInit();

	/*
	const GLubyte *renderer;
	const GLubyte *version;
	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);
	(*log) << "Renderer: ";
	(*log) << (const char*)renderer;
	(*log) << "\nOpenGL version supported ";
	(*log) << (const char*)version;
	(*log) << "\n\n";
	*/

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
	//glClearColor(0.0f, 0.0f, 0.0f, 0.5f);           // Black Background
	//glClearDepth(1.0f);                             // Depth Buffer Setup

	GLfloat points[] = {
		-1.0,  -1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,
		 1.0f,	1.0f,  0.0f,
		 1.0f,	1.0f,  0.0f,
		-1.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f };

	GLfloat texcoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f };

	glhScreen.SetVBO(points, 18, 0, 3);
	glhScreen.SetVBO(texcoords, 12, 1, 2);
	glhScreen.SetVAO();

	shader_programme = GLUtils::loadShaderProg(vertexShaderCode,
	                                           fragmentShaderCode);

	glUseProgram(shader_programme);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	return true;
}
void GLScene2D::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	int offset = 3 * (x + RES_X * (RES_Y - y - 1));
	pixels[offset]     = r;
	pixels[offset + 1] = g;
	pixels[offset + 2] = b;
	shouldScreenUpdate = true;
}
bool GLScene2D::DrawGLScene(GLFWwindow* window, int w_width, int w_height) {
	if (shouldScreenUpdate) {
		shouldScreenUpdate = false;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		double arScreen = RES_X * 1.0 / RES_Y;
		double arWindow = w_width * 1.0 / w_height;
		if (arScreen > arWindow) { // Letterboxing
			int height = static_cast<int>(w_width * 1.0 / arScreen);
			int offset_y = static_cast<int>((w_height - height) * 1.0 / 2.0);
			glViewport(0, offset_y, w_width, height);
		}
		else { // Pillarboxing
			int width = static_cast<int>(w_height * arScreen);
			int offset_x = static_cast<int>((w_width - width) * 1.0 / 2.0);
			glViewport(offset_x, 0, width, w_height);
		}
		glhScreen.LoadTexture(pixels, RES_X, RES_Y, 0);
		glhScreen.BindAndDraw(shader_programme);
		glfwSwapBuffers(window);
		return true;
	}
	return false;
}
std::string GLScene2D::GetScreenHash() {
	// https://gist.github.com/nitrix/34196ff0c93fdfb01d51
	uint32_t magic = 5381;
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			int offset = 3 * (x + RES_X * (RES_Y - y - 1));
			for (int i = 0; i < 3; i++) {
				uint8_t c = pixels[offset + i];
				magic = ((magic << 5) + magic) + c; // magic * 33 + c
			}
		}
	}
	std::stringstream ss;
	ss << std::hex << std::setw(8) << std::setfill('0') << magic;
	return ss.str();
}

GLScene2D::GLScene2D(int _RES_X, int _RES_Y) {
	RES_X = _RES_X;
	RES_Y = _RES_Y;
	pixels = new uint8_t[3 * RES_X * RES_Y];
	SetupPixels();
}
GLScene2D::~GLScene2D() {
	delete[] pixels;
}
