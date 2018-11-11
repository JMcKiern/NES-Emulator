#pragma once

#include <string>
#include <GL\glew.h>
#include "maths_funcs.h"

// Collection of general functions to help with OpenGL

mat4 rotateAboutAxis(double ang_deg, vec3 axis);
unsigned char* load_texture(const char *file_name, int& _width, int& _height);
std::string readShaderFile(std::string fileName);
GLuint loadShaderProg(std::string vertexShaderFile,
                      std::string fragmentShaderFile);