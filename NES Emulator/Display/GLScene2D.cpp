#include <sstream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "stb_image.h"
#include "GLScene2D.h"
#include "../Log.h"

GLuint GLScene2D::loadShaderProg(std::string vertexShaderFile, std::string fragmentShaderFile) {
	GLuint sProg;
	std::string vs = readShaderFile(vertexShaderFile);
	const char *vertex_shader = vs.c_str();
	std::string fs = readShaderFile(fragmentShaderFile);
	const char *fragment_shader = fs.c_str();
	GLuint vert_shader, frag_shader;
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vertex_shader, NULL);
	glCompileShader(vert_shader);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fragment_shader, NULL);
	glCompileShader(frag_shader);
	sProg = glCreateProgram();
	glAttachShader(sProg, frag_shader);
	glAttachShader(sProg, vert_shader);
	glLinkProgram(sProg);
	return sProg;
}
void GLScene2D::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	int offset = 3 * (x + RES_X * (RES_Y - y - 1));
	pixels[offset]     = r;
	pixels[offset + 1] = g;
	pixels[offset + 2] = b;
	shouldScreenUpdate = true;
}

bool GLScene2D::InitGL() {
	// Setup OpenGL
	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte *renderer;
	const GLubyte *version;
	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);
	(*log) << "Renderer: ";
	(*log) << (const char*)renderer;
	(*log) << "\nOpenGL version supported ";
	(*log) << (const char*)version;
	(*log) << "\n\n";

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
	//glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               // Black Background
	//glClearDepth(1.0f);                                 // Depth Buffer Setup

	GLuint points_vbo;
	GLuint vt_vbo;

	GLfloat points[] = {
		-1.0,  -1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,
		 1.0f,	1.0f,  0.0f,
		 1.0f,	1.0f,  0.0f,
		-1.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f };

	for (int i = 0; i < 18; ++i) {
		if ((i - 1) % 3 == 0)
			points[i] /= (float) RES_X * 1.0 / RES_Y;
		points[i] *= (float)scaleFactor;
	}

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

	shader_programme = loadShaderProg(vertexShaderFile, fragmentShaderFile);

	glUseProgram(shader_programme);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	return true;                                        // Initialization Went OK
}

bool GLScene2D::DrawGLScene(GLFWwindow* window, int w_width, int w_height) {
	if (!shouldScreenUpdate) {
		return false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w_width, w_height);
	shouldScreenUpdate = false;
	glhScreen.LoadTexture(pixels, RES_X, RES_Y, 0);
	glhScreen.BindAndDraw(shader_programme);
	glfwSwapBuffers(window);
	return true;
}

GLScene2D::GLScene2D(Log* _log) {
	log = _log;
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			uint8_t r, g, b, a;
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