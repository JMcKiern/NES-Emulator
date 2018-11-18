#pragma once

#include <string>
#include <GL/glew.h>
#include "maths_funcs.h"

// Collection of general functions to help with OpenGL

namespace GLUtils {
	mat4 rotateAboutAxis(double ang_deg, vec3 axis);
	std::string readShaderFile(std::string fileName);
	GLuint loadShaderFile(std::string vertexShaderFile,
	                      std::string fragmentShaderFile);
	GLuint loadShaderProg(std::string vertex_shader,
	                      std::string fragment_shader);
	void checkForShaderCompilationErrors(std::string shader_code,
	                                     GLuint shader);
} // GLUtils
