#pragma once

#include <string>
#include <GL/glew.h>
#include "maths_funcs.h"

// Collection of general functions to help with OpenGL

namespace GLUtils {
	mat4 rotateAboutAxis(double ang_deg, vec3 axis);
	std::string readShaderFile(const std::string& fileName);
	GLuint loadShaderFile(const std::string& vertexShaderFile,
	                      const std::string& fragmentShaderFile);
	GLuint loadShaderProg(const std::string& vertex_shader,
	                      const std::string& fragment_shader);
	void checkForShaderCompilationErrors(const std::string& shader_code,
	                                     GLuint shader);
} // GLUtils
