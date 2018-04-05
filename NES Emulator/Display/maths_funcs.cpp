/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries' separate legal notices                             |
|******************************************************************************|
| Commonly-used maths structures and functions                                 |
| Simple-as-possible. No disgusting templates.                                 |
| Structs vec3, mat4, versor. just hold arrays of doubles called "v","m","q",   |
| respectively. So, for example, to get values from a mat4 do: my_mat.m        |
| A versor is the proper name for a unit quaternion.                           |
\******************************************************************************/
#include "maths_funcs.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

/*--------------------------------CONSTRUCTORS--------------------------------*/
vec2::vec2() {}

vec2::vec2(double x, double y) {
	v[0] = x;
	v[1] = y;
}

vec3::vec3() {}

vec3::vec3(double x, double y, double z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

vec3::vec3(const vec2 &vv, double z) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
}

vec3::vec3(const vec4 &vv) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
}

vec4::vec4() {}

vec4::vec4(double x, double y, double z, double w) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

vec4::vec4(const vec2 &vv, double z, double w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
	v[3] = w;
}

vec4::vec4(const vec3 &vv, double w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
	v[3] = w;
}

mat3::mat3() {}

/* note: entered in COLUMNS */
mat3::mat3(double a, double b, double c, double d, double e, double f, double g, double h,
	double i) {
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
}

mat4::mat4() {}

/* note: entered in COLUMNS */
mat4::mat4(double a, double b, double c, double d, double e, double f, double g, double h,
	double i, double j, double k, double l, double mm, double n, double o,
	double p) {
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
	m[9] = j;
	m[10] = k;
	m[11] = l;
	m[12] = mm;
	m[13] = n;
	m[14] = o;
	m[15] = p;
}

/*-----------------------------PRINT FUNCTIONS--------------------------------*/
void print(const vec2 &v) { printf("[%.2f, %.2f]\n", v.v[0], v.v[1]); }

void print(const vec3 &v) {
	printf("[%.2f, %.2f, %.2f]\n", v.v[0], v.v[1], v.v[2]);
}

void print(const vec4 &v) {
	printf("[%.2f, %.2f, %.2f, %.2f]\n", v.v[0], v.v[1], v.v[2], v.v[3]);
}

void print(const mat3 &m) {
	printf("\n");
	printf("[%.2f][%.2f][%.2f]\n", m.m[0], m.m[3], m.m[6]);
	printf("[%.2f][%.2f][%.2f]\n", m.m[1], m.m[4], m.m[7]);
	printf("[%.2f][%.2f][%.2f]\n", m.m[2], m.m[5], m.m[8]);
}

void print(const mat4 &m) {
	printf("\n");
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[0], m.m[4], m.m[8], m.m[12]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[1], m.m[5], m.m[9], m.m[13]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[2], m.m[6], m.m[10], m.m[14]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[3], m.m[7], m.m[11], m.m[15]);
}

/*------------------------------VECTOR FUNCTIONS------------------------------*/
double length(const vec3 &v) {
	return sqrt(v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
}

// squared length
double length2(const vec3 &v) {
	return v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
}

// note: proper spelling (hehe)
vec3 normalise(const vec3 &v) {
	vec3 vb;
	double l = length(v);
	if (0.0f == l) {
		return vec3(0.0f, 0.0f, 0.0f);
	}
	vb.v[0] = v.v[0] / l;
	vb.v[1] = v.v[1] / l;
	vb.v[2] = v.v[2] / l;
	return vb;
}

vec3 vec3::operator+(const vec3 &rhs) {
	vec3 vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	vc.v[2] = v[2] + rhs.v[2];
	return vc;
}

vec3 &vec3::operator+=(const vec3 &rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	v[2] += rhs.v[2];
	return *this; // return self
}

vec3 vec3::operator-(const vec3 &rhs) {
	vec3 vc;
	vc.v[0] = v[0] - rhs.v[0];
	vc.v[1] = v[1] - rhs.v[1];
	vc.v[2] = v[2] - rhs.v[2];
	return vc;
}

vec3 &vec3::operator-=(const vec3 &rhs) {
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	v[2] -= rhs.v[2];
	return *this;
}

vec3 vec3::operator+(double rhs) {
	vec3 vc;
	vc.v[0] = v[0] + rhs;
	vc.v[1] = v[1] + rhs;
	vc.v[2] = v[2] + rhs;
	return vc;
}

vec3 vec3::operator-(double rhs) {
	vec3 vc;
	vc.v[0] = v[0] - rhs;
	vc.v[1] = v[1] - rhs;
	vc.v[2] = v[2] - rhs;
	return vc;
}

vec3 vec3::operator*(double rhs) {
	vec3 vc;
	vc.v[0] = v[0] * rhs;
	vc.v[1] = v[1] * rhs;
	vc.v[2] = v[2] * rhs;
	return vc;
}

vec3 vec3::operator/(double rhs) {
	vec3 vc;
	vc.v[0] = v[0] / rhs;
	vc.v[1] = v[1] / rhs;
	vc.v[2] = v[2] / rhs;
	return vc;
}

vec3 &vec3::operator*=(double rhs) {
	v[0] = v[0] * rhs;
	v[1] = v[1] * rhs;
	v[2] = v[2] * rhs;
	return *this;
}

vec3 &vec3::operator=(const vec3 &rhs) {
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
	return *this;
}

double dot(const vec3 &a, const vec3 &b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}

vec3 cross(const vec3 &a, const vec3 &b) {
	double x = a.v[1] * b.v[2] - a.v[2] * b.v[1];
	double y = a.v[2] * b.v[0] - a.v[0] * b.v[2];
	double z = a.v[0] * b.v[1] - a.v[1] * b.v[0];
	return vec3(x, y, z);
}

double get_squared_dist(vec3 from, vec3 to) {
	double x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	double y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	double z = (to.v[2] - from.v[2]) * (to.v[2] - from.v[2]);
	return x + y + z;
}

/* converts an un-normalised direction into a heading in degrees
NB i suspect that the z is backwards here but i've used in in
several places like this. d'oh! */
double direction_to_heading(vec3 d) {
	return atan2(-d.v[0], -d.v[2]) * ONE_RAD_IN_DEG;
}

vec3 heading_to_direction(double degrees) {
	double rad = degrees * ONE_DEG_IN_RAD;
	return vec3(-sin(rad), 0.0f, -cos(rad));
}

/*-----------------------------MATRIX FUNCTIONS-------------------------------*/
mat3 zero_mat3() {
	return mat3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

mat3 identity_mat3() {
	return mat3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 zero_mat4() {
	return mat4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

mat4 identity_mat4() {
	return mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

/* mat4 array layout
0  4  8 12
1  5  9 13
2  6 10 14
3  7 11 15
*/

vec4 mat4::operator*(const vec4 &rhs) {
	// 0x + 4y + 8z + 12w
	double x = m[0] * rhs.v[0] + m[4] * rhs.v[1] + m[8] * rhs.v[2] + m[12] * rhs.v[3];
	// 1x + 5y + 9z + 13w
	double y = m[1] * rhs.v[0] + m[5] * rhs.v[1] + m[9] * rhs.v[2] + m[13] * rhs.v[3];
	// 2x + 6y + 10z + 14w
	double z = m[2] * rhs.v[0] + m[6] * rhs.v[1] + m[10] * rhs.v[2] + m[14] * rhs.v[3];
	// 3x + 7y + 11z + 15w
	double w = m[3] * rhs.v[0] + m[7] * rhs.v[1] + m[11] * rhs.v[2] + m[15] * rhs.v[3];
	return vec4(x, y, z, w);
}

mat4 mat4::operator*(const mat4 &rhs) {
	mat4 r = zero_mat4();
	int r_index = 0;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			double sum = 0.0f;
			for (int i = 0; i < 4; i++) {
				sum += rhs.m[i + col * 4] * m[row + i * 4];
			}
			r.m[r_index] = sum;
			r_index++;
		}
	}
	return r;
}

mat4 &mat4::operator=(const mat4 &rhs) {
	for (int i = 0; i < 16; i++) {
		m[i] = rhs.m[i];
	}
	return *this;
}

// returns a scalar value with the determinant for a 4x4 matrix
// see
// http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
double determinant(const mat4 &mm) {
	return mm.m[12] * mm.m[9] * mm.m[6] * mm.m[3] -
		mm.m[8] * mm.m[13] * mm.m[6] * mm.m[3] -
		mm.m[12] * mm.m[5] * mm.m[10] * mm.m[3] +
		mm.m[4] * mm.m[13] * mm.m[10] * mm.m[3] +
		mm.m[8] * mm.m[5] * mm.m[14] * mm.m[3] -
		mm.m[4] * mm.m[9] * mm.m[14] * mm.m[3] -
		mm.m[12] * mm.m[9] * mm.m[2] * mm.m[7] +
		mm.m[8] * mm.m[13] * mm.m[2] * mm.m[7] +
		mm.m[12] * mm.m[1] * mm.m[10] * mm.m[7] -
		mm.m[0] * mm.m[13] * mm.m[10] * mm.m[7] -
		mm.m[8] * mm.m[1] * mm.m[14] * mm.m[7] +
		mm.m[0] * mm.m[9] * mm.m[14] * mm.m[7] +
		mm.m[12] * mm.m[5] * mm.m[2] * mm.m[11] -
		mm.m[4] * mm.m[13] * mm.m[2] * mm.m[11] -
		mm.m[12] * mm.m[1] * mm.m[6] * mm.m[11] +
		mm.m[0] * mm.m[13] * mm.m[6] * mm.m[11] +
		mm.m[4] * mm.m[1] * mm.m[14] * mm.m[11] -
		mm.m[0] * mm.m[5] * mm.m[14] * mm.m[11] -
		mm.m[8] * mm.m[5] * mm.m[2] * mm.m[15] +
		mm.m[4] * mm.m[9] * mm.m[2] * mm.m[15] +
		mm.m[8] * mm.m[1] * mm.m[6] * mm.m[15] -
		mm.m[0] * mm.m[9] * mm.m[6] * mm.m[15] -
		mm.m[4] * mm.m[1] * mm.m[10] * mm.m[15] +
		mm.m[0] * mm.m[5] * mm.m[10] * mm.m[15];
}

/* returns a 16-element array that is the inverse of a 16-element array (4x4
matrix). see
http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
*/
mat4 inverse(const mat4 &mm) {
	double det = determinant(mm);
	/* there is no inverse if determinant is zero (not likely unless scale is
	broken) */
	if (0.0f == det) {
		fprintf(stderr, "WARNING. matrix has no determinant. can not invert\n");
		return mm;
	}
	double inv_det = 1.0f / det;

	return mat4(
		inv_det * (mm.m[9] * mm.m[14] * mm.m[7] - mm.m[13] * mm.m[10] * mm.m[7] +
			mm.m[13] * mm.m[6] * mm.m[11] - mm.m[5] * mm.m[14] * mm.m[11] -
			mm.m[9] * mm.m[6] * mm.m[15] + mm.m[5] * mm.m[10] * mm.m[15]),
		inv_det * (mm.m[13] * mm.m[10] * mm.m[3] - mm.m[9] * mm.m[14] * mm.m[3] -
			mm.m[13] * mm.m[2] * mm.m[11] + mm.m[1] * mm.m[14] * mm.m[11] +
			mm.m[9] * mm.m[2] * mm.m[15] - mm.m[1] * mm.m[10] * mm.m[15]),
		inv_det * (mm.m[5] * mm.m[14] * mm.m[3] - mm.m[13] * mm.m[6] * mm.m[3] +
			mm.m[13] * mm.m[2] * mm.m[7] - mm.m[1] * mm.m[14] * mm.m[7] -
			mm.m[5] * mm.m[2] * mm.m[15] + mm.m[1] * mm.m[6] * mm.m[15]),
		inv_det * (mm.m[9] * mm.m[6] * mm.m[3] - mm.m[5] * mm.m[10] * mm.m[3] -
			mm.m[9] * mm.m[2] * mm.m[7] + mm.m[1] * mm.m[10] * mm.m[7] +
			mm.m[5] * mm.m[2] * mm.m[11] - mm.m[1] * mm.m[6] * mm.m[11]),
		inv_det * (mm.m[12] * mm.m[10] * mm.m[7] - mm.m[8] * mm.m[14] * mm.m[7] -
			mm.m[12] * mm.m[6] * mm.m[11] + mm.m[4] * mm.m[14] * mm.m[11] +
			mm.m[8] * mm.m[6] * mm.m[15] - mm.m[4] * mm.m[10] * mm.m[15]),
		inv_det * (mm.m[8] * mm.m[14] * mm.m[3] - mm.m[12] * mm.m[10] * mm.m[3] +
			mm.m[12] * mm.m[2] * mm.m[11] - mm.m[0] * mm.m[14] * mm.m[11] -
			mm.m[8] * mm.m[2] * mm.m[15] + mm.m[0] * mm.m[10] * mm.m[15]),
		inv_det * (mm.m[12] * mm.m[6] * mm.m[3] - mm.m[4] * mm.m[14] * mm.m[3] -
			mm.m[12] * mm.m[2] * mm.m[7] + mm.m[0] * mm.m[14] * mm.m[7] +
			mm.m[4] * mm.m[2] * mm.m[15] - mm.m[0] * mm.m[6] * mm.m[15]),
		inv_det * (mm.m[4] * mm.m[10] * mm.m[3] - mm.m[8] * mm.m[6] * mm.m[3] +
			mm.m[8] * mm.m[2] * mm.m[7] - mm.m[0] * mm.m[10] * mm.m[7] -
			mm.m[4] * mm.m[2] * mm.m[11] + mm.m[0] * mm.m[6] * mm.m[11]),
		inv_det * (mm.m[8] * mm.m[13] * mm.m[7] - mm.m[12] * mm.m[9] * mm.m[7] +
			mm.m[12] * mm.m[5] * mm.m[11] - mm.m[4] * mm.m[13] * mm.m[11] -
			mm.m[8] * mm.m[5] * mm.m[15] + mm.m[4] * mm.m[9] * mm.m[15]),
		inv_det * (mm.m[12] * mm.m[9] * mm.m[3] - mm.m[8] * mm.m[13] * mm.m[3] -
			mm.m[12] * mm.m[1] * mm.m[11] + mm.m[0] * mm.m[13] * mm.m[11] +
			mm.m[8] * mm.m[1] * mm.m[15] - mm.m[0] * mm.m[9] * mm.m[15]),
		inv_det * (mm.m[4] * mm.m[13] * mm.m[3] - mm.m[12] * mm.m[5] * mm.m[3] +
			mm.m[12] * mm.m[1] * mm.m[7] - mm.m[0] * mm.m[13] * mm.m[7] -
			mm.m[4] * mm.m[1] * mm.m[15] + mm.m[0] * mm.m[5] * mm.m[15]),
		inv_det * (mm.m[8] * mm.m[5] * mm.m[3] - mm.m[4] * mm.m[9] * mm.m[3] -
			mm.m[8] * mm.m[1] * mm.m[7] + mm.m[0] * mm.m[9] * mm.m[7] +
			mm.m[4] * mm.m[1] * mm.m[11] - mm.m[0] * mm.m[5] * mm.m[11]),
		inv_det * (mm.m[12] * mm.m[9] * mm.m[6] - mm.m[8] * mm.m[13] * mm.m[6] -
			mm.m[12] * mm.m[5] * mm.m[10] + mm.m[4] * mm.m[13] * mm.m[10] +
			mm.m[8] * mm.m[5] * mm.m[14] - mm.m[4] * mm.m[9] * mm.m[14]),
		inv_det * (mm.m[8] * mm.m[13] * mm.m[2] - mm.m[12] * mm.m[9] * mm.m[2] +
			mm.m[12] * mm.m[1] * mm.m[10] - mm.m[0] * mm.m[13] * mm.m[10] -
			mm.m[8] * mm.m[1] * mm.m[14] + mm.m[0] * mm.m[9] * mm.m[14]),
		inv_det * (mm.m[12] * mm.m[5] * mm.m[2] - mm.m[4] * mm.m[13] * mm.m[2] -
			mm.m[12] * mm.m[1] * mm.m[6] + mm.m[0] * mm.m[13] * mm.m[6] +
			mm.m[4] * mm.m[1] * mm.m[14] - mm.m[0] * mm.m[5] * mm.m[14]),
		inv_det * (mm.m[4] * mm.m[9] * mm.m[2] - mm.m[8] * mm.m[5] * mm.m[2] +
			mm.m[8] * mm.m[1] * mm.m[6] - mm.m[0] * mm.m[9] * mm.m[6] -
			mm.m[4] * mm.m[1] * mm.m[10] + mm.m[0] * mm.m[5] * mm.m[10]));
}

// returns a 16-element array flipped on the main diagonal
mat4 transpose(const mat4 &mm) {
	return mat4(mm.m[0], mm.m[4], mm.m[8], mm.m[12], mm.m[1], mm.m[5], mm.m[9],
		mm.m[13], mm.m[2], mm.m[6], mm.m[10], mm.m[14], mm.m[3], mm.m[7],
		mm.m[11], mm.m[15]);
}

/*--------------------------AFFINE MATRIX FUNCTIONS---------------------------*/
// translate a 4d matrix with xyz array
mat4 translate(const mat4 &m, const vec3 &v) {
	mat4 m_t = identity_mat4();
	m_t.m[12] = v.v[0];
	m_t.m[13] = v.v[1];
	m_t.m[14] = v.v[2];
	return m_t * m;
}

// rotate around x axis by an angle in degrees
mat4 rotate_x_deg(const mat4 &m, double deg) {
	// convert to radians
	double rad = deg * ONE_DEG_IN_RAD;
	mat4 m_r = identity_mat4();
	m_r.m[5] = cos(rad);
	m_r.m[9] = -sin(rad);
	m_r.m[6] = sin(rad);
	m_r.m[10] = cos(rad);
	return m_r * m;
}

// rotate around y axis by an angle in degrees
mat4 rotate_y_deg(const mat4 &m, double deg) {
	// convert to radians
	double rad = deg * ONE_DEG_IN_RAD;
	mat4 m_r = identity_mat4();
	m_r.m[0] = cos(rad);
	m_r.m[8] = sin(rad);
	m_r.m[2] = -sin(rad);
	m_r.m[10] = cos(rad);
	return m_r * m;
}

// rotate around z axis by an angle in degrees
mat4 rotate_z_deg(const mat4 &m, double deg) {
	// convert to radians
	double rad = deg * ONE_DEG_IN_RAD;
	mat4 m_r = identity_mat4();
	m_r.m[0] = cos(rad);
	m_r.m[4] = -sin(rad);
	m_r.m[1] = sin(rad);
	m_r.m[5] = cos(rad);
	return m_r * m;
}

// scale a matrix by [x, y, z]
mat4 scale(const mat4 &m, const vec3 &v) {
	mat4 a = identity_mat4();
	a.m[0] = v.v[0];
	a.m[5] = v.v[1];
	a.m[10] = v.v[2];
	return a * m;
}

/*-----------------------VIRTUAL CAMERA MATRIX FUNCTIONS----------------------*/
// returns a view matrix using the opengl lookAt style. COLUMN ORDER.
mat4 look_at(const vec3 &cam_pos, vec3 targ_pos, const vec3 &up) {
	// inverse translation
	mat4 p = identity_mat4();
	p = translate(p, vec3(-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]));
	// distance vector
	vec3 d = targ_pos - cam_pos;
	// forward vector
	vec3 f = normalise(d);
	// right vector
	vec3 r = normalise(cross(f, up));
	// real up vector
	vec3 u = normalise(cross(r, f));
	mat4 ori = identity_mat4();
	ori.m[0] = r.v[0];
	ori.m[4] = r.v[1];
	ori.m[8] = r.v[2];
	ori.m[1] = u.v[0];
	ori.m[5] = u.v[1];
	ori.m[9] = u.v[2];
	ori.m[2] = -f.v[0];
	ori.m[6] = -f.v[1];
	ori.m[10] = -f.v[2];

	return ori * p; // p * ori;
}

// returns a perspective function mimicking the opengl projection style.
mat4 perspective(double fovy, double aspect, double near, double far) {
	double fov_rad = fovy * ONE_DEG_IN_RAD;
	double range = tan(fov_rad / 2.0f) * near;
	double sx = (2.0f * near) / (range * aspect + range * aspect);
	double sy = near / range;
	double sz = -(far + near) / (far - near);
	double pz = -(2.0f * far * near) / (far - near);
	mat4 m = zero_mat4(); // make sure bottom-right corner is zero
	m.m[0] = sx;
	m.m[5] = sy;
	m.m[10] = sz;
	m.m[14] = pz;
	m.m[11] = -1.0f;
	return m;
}

/*----------------------------HAMILTON IN DA HOUSE!---------------------------*/
versor::versor() {}

versor versor::operator/(double rhs) {
	versor result;
	result.q[0] = q[0] / rhs;
	result.q[1] = q[1] / rhs;
	result.q[2] = q[2] / rhs;
	result.q[3] = q[3] / rhs;
	return result;
}

versor versor::operator*(double rhs) {
	versor result;
	result.q[0] = q[0] * rhs;
	result.q[1] = q[1] * rhs;
	result.q[2] = q[2] * rhs;
	result.q[3] = q[3] * rhs;
	return result;
}

void print(const versor &q) {
	printf("[%.2f ,%.2f, %.2f, %.2f]\n", q.q[0], q.q[1], q.q[2], q.q[3]);
}

versor versor::operator*(const versor &rhs) {
	versor result;
	result.q[0] =
		rhs.q[0] * q[0] - rhs.q[1] * q[1] - rhs.q[2] * q[2] - rhs.q[3] * q[3];
	result.q[1] =
		rhs.q[0] * q[1] + rhs.q[1] * q[0] - rhs.q[2] * q[3] + rhs.q[3] * q[2];
	result.q[2] =
		rhs.q[0] * q[2] + rhs.q[1] * q[3] + rhs.q[2] * q[0] - rhs.q[3] * q[1];
	result.q[3] =
		rhs.q[0] * q[3] - rhs.q[1] * q[2] + rhs.q[2] * q[1] + rhs.q[3] * q[0];
	// re-normalise in case of mangling
	return normalise(result);
}

versor versor::operator+(const versor &rhs) {
	versor result;
	result.q[0] = rhs.q[0] + q[0];
	result.q[1] = rhs.q[1] + q[1];
	result.q[2] = rhs.q[2] + q[2];
	result.q[3] = rhs.q[3] + q[3];
	// re-normalise in case of mangling
	return normalise(result);
}

versor quat_from_axis_rad(double radians, double x, double y, double z) {
	versor result;
	result.q[0] = cos(radians / 2.0);
	result.q[1] = sin(radians / 2.0) * x;
	result.q[2] = sin(radians / 2.0) * y;
	result.q[3] = sin(radians / 2.0) * z;
	return result;
}

versor quat_from_axis_deg(double degrees, double x, double y, double z) {
	return quat_from_axis_rad(ONE_DEG_IN_RAD * degrees, x, y, z);
}

mat4 quat_to_mat4(const versor &q) {
	double w = q.q[0];
	double x = q.q[1];
	double y = q.q[2];
	double z = q.q[3];
	return mat4(1.0f - 2.0f * y * y - 2.0f * z * z, 2.0f * x * y + 2.0f * w * z,
		2.0f * x * z - 2.0f * w * y, 0.0f, 2.0f * x * y - 2.0f * w * z,
		1.0f - 2.0f * x * x - 2.0f * z * z, 2.0f * y * z + 2.0f * w * x,
		0.0f, 2.0f * x * z + 2.0f * w * y, 2.0f * y * z - 2.0f * w * x,
		1.0f - 2.0f * x * x - 2.0f * y * y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

versor normalise(versor &q) {
	// norm(q) = q / magnitude (q)
	// magnitude (q) = sqrt (w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	double sum = q.q[0] * q.q[0] + q.q[1] * q.q[1] + q.q[2] * q.q[2] + q.q[3] * q.q[3];
	// NB: doubles have min 6 digits of precision
	const double thresh = 0.0001f;
	if (fabs(1.0f - sum) < thresh) {
		return q;
	}
	double mag = sqrt(sum);
	return q / mag;
}

double dot(const versor &q, const versor &r) {
	return q.q[0] * r.q[0] + q.q[1] * r.q[1] + q.q[2] * r.q[2] + q.q[3] * r.q[3];
}

versor slerp(versor &q, versor &r, double t) {
	// angle between q0-q1
	double cos_half_theta = dot(q, r);
	// as found here
	// http://stackoverflow.com/questions/2886606/flipping-issue-when-interpolating-rotations-using-quaternions
	// if dot product is negative then one quaternion should be negated, to make
	// it take the short way around, rather than the long way
	// yeah! and furthermore Susan, I had to recalculate the d.p. after this
	if (cos_half_theta < 0.0f) {
		for (int i = 0; i < 4; i++) {
			q.q[i] *= -1.0f;
		}
		cos_half_theta = dot(q, r);
	}
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (fabs(cos_half_theta) >= 1.0f) {
		return q;
	}
	// Calculate temporary values
	double sin_half_theta = sqrt(1.0f - cos_half_theta * cos_half_theta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	versor result;
	if (fabs(sin_half_theta) < 0.001f) {
		for (int i = 0; i < 4; i++) {
			result.q[i] = (1.0f - t) * q.q[i] + t * r.q[i];
		}
		return result;
	}
	double half_theta = acos(cos_half_theta);
	double a = sin((1.0f - t) * half_theta) / sin_half_theta;
	double b = sin(t * half_theta) / sin_half_theta;
	for (int i = 0; i < 4; i++) {
		result.q[i] = q.q[i] * a + r.q[i] * b;
	}
	return result;
}