#include <math.h>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include "maths_funcs.h"
#include "stb_image.h"


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
unsigned char* load_texture(const char *file_name, int& _width, int& _height) {
	int x, y, n;
	int force_channels = 4;
	unsigned char *image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr, "WARNING: texture %s is not power-of-2 dimensions\n",
			file_name);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	_width = x;
	_height = y;
	return image_data;
}
std::string readShaderFile(std::string fileName) {
	std::ifstream f;
	f.open(fileName);
	if (!f.is_open()) {
		throw std::exception("Could not open shader file");
	}

	// https://stackoverflow.com/questions/116038/what-is-the-best-way-to-read-an-entire-file-into-a-stdstring-in-c
	std::stringstream sstr;
	sstr << f.rdbuf();
	f.close();
	return sstr.str();
}
