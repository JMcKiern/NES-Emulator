#include <GL/glew.h>
#include <math.h>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "GLUtils.h"
#include "maths_funcs.h"

namespace GLUtils {
	mat4 rotateAboutAxis(double ang_deg, vec3 axis) {
		double ang_rad = ang_deg * ONE_DEG_IN_RAD;
		axis = normalise(axis);
		double u_x = axis.v[0];
		double u_y = axis.v[1];
		double u_z = axis.v[2];
		mat4 R = identity_mat4();
		R.m[0] = cos(ang_rad) + u_x * u_x * (1 - cos(ang_rad));
		R.m[1] = u_x * u_y * (1 - cos(ang_rad)) - u_z * sin(ang_rad);
		R.m[2] = u_x * u_z * (1 - cos(ang_rad)) + u_y * sin(ang_rad);
		R.m[4] = u_y * u_x * (1 - cos(ang_rad)) + u_z * sin(ang_rad);
		R.m[5] = cos(ang_rad) + u_y * u_y * (1 - cos(ang_rad));
		R.m[6] = u_y * u_z * (1 - cos(ang_rad)) - u_x * sin(ang_rad);
		R.m[8] = u_z * u_x * (1 - cos(ang_rad)) - u_y * sin(ang_rad);
		R.m[9] = u_z * u_y * (1 - cos(ang_rad)) + u_x * sin(ang_rad);
		R.m[10] = cos(ang_rad) + u_z * u_z * (1 - cos(ang_rad));
		return R;
	}
	std::string readShaderFile(const std::string& fileName) {
		std::ifstream f;
		f.open(fileName);
		if (!f.is_open()) {
			throw std::runtime_error("Could not open shader file");
		}

		// https://stackoverflow.com/questions/116038/what-is-the-best-way-to-read-an-entire-file-into-a-stdstring-in-c
		std::stringstream sstr;
		sstr << f.rdbuf();
		f.close();
		return sstr.str();
	}
	GLuint loadShaderFile(const std::string& vertexShaderFile,
	                      const std::string& fragmentShaderFile) {

		std::string vs = readShaderFile(vertexShaderFile);
		const char* vertex_shader = vs.c_str();

		std::string fs = readShaderFile(fragmentShaderFile);
		const char* fragment_shader = fs.c_str();

		return loadShaderProg(vertex_shader, fragment_shader);
	}

	GLuint loadShaderProg(const std::string& vertex_shader,
	                      const std::string& fragment_shader) {
		GLuint vert_shader, frag_shader;

		vert_shader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertex_shader_cstr = vertex_shader.c_str();
		glShaderSource(vert_shader, 1, &vertex_shader_cstr, NULL);
		glCompileShader(vert_shader);
		checkForShaderCompilationErrors(vertex_shader, vert_shader);

		frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragment_shader_cstr = fragment_shader.c_str();
		glShaderSource(frag_shader, 1, &fragment_shader_cstr, NULL);
		glCompileShader(frag_shader);
		checkForShaderCompilationErrors(fragment_shader, frag_shader);

		GLuint sProg = glCreateProgram();
		glAttachShader(sProg, frag_shader);
		glAttachShader(sProg, vert_shader);
		glLinkProgram(sProg);

		return sProg;
	}

	void checkForShaderCompilationErrors(const std::string& shader_code,
	                                     GLuint shader) {
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			std::cout << "ShaderCompilation ERROR: " << shader_code << '\n';
			for (auto it = errorLog.begin(); it != errorLog.end(); ++it) {
				std::cout << (*it);
			}

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(shader); // Don't leak the shader.
			throw std::runtime_error("Shader did not compile");
		}
	}
} // GLUtils