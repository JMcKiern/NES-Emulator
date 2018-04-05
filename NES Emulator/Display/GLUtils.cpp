#include <math.h>
#include "maths_funcs.h"

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