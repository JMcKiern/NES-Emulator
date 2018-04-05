#include "Camera.h"
#include <iostream>
#include <math.h>
#include "GLUtils.h"
#include "maths_funcs.h"
void Camera::MouseMove(long dx, long dy) {
	yaw -= dx * m_yaw * sens;
	pitch -= dy * m_pitch * sens;

	if (pitch > 90)
		pitch = 90;
	if (pitch < -90)
		pitch = -90;
}
void Camera::PrintPos() {
	std::cout << pos.v[0] << ", "
	 << pos.v[1] << ", "
	 << pos.v[2] << "\n";
}
void Camera::MoveDir(vec3 dir, double magnitude) {
	dir = normalise(dir);
	pos += dir * magnitude * moveSpeed;
}
void Camera::MoveVect(vec3 vect) {
	pos += vect * moveSpeed;
}

mat4 Camera::GetViewMat() {
	
	UpdateViewMat();
	return viewMat;
}
void Camera::SetViewMat(mat4 _viewMat) {
	viewMat = _viewMat;
}
void Camera::UpdateViewMat() {
	rgt = vec3(1.0f * cos(yaw * ONE_DEG_IN_RAD), 0.0f, -1.0f * sin(yaw * ONE_DEG_IN_RAD));
	mat4 T = translate(identity_mat4(), vec3(-pos.v[0], -pos.v[1], -pos.v[2]));
	mat4 R_yaw = rotateAboutAxis(yaw, up);
	mat4 R_pitch = rotateAboutAxis(pitch, rgt);
	viewMat = R_yaw * R_pitch * T;
}

Camera::Camera(vec3 _pos, double _pitch, double _yaw, double _sens, double _m_yaw, double _m_pitch) {
	pos = _pos;
	pitch = _pitch;
	yaw = _yaw;
	sens = _sens;
	m_yaw = _m_yaw;
	m_pitch = _m_pitch;
	rgt = vec3(1.0f * cos(yaw * ONE_DEG_IN_RAD), 0.0f, 1.0f * sin(yaw * ONE_DEG_IN_RAD));
	up = vec3(0.0f, 1.0f, 0.0f);
}