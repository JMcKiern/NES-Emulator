#pragma once

#include <string>
#include "maths_funcs.h"

mat4 rotateAboutAxis(double ang_deg, vec3 axis);
unsigned char* load_texture(const char *file_name, int& _width, int& _height);
std::string readShaderFile(std::string fileName);