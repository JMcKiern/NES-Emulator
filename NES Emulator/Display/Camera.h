#pragma once

#include "maths_funcs.h"

// Implementation of a simple FPS Camera 
// TODO: 
//		Add if statement to only update viewMat when necessary
//		Make Click more general so it applies to all FPS camera implementations
//		Fix Click
class Camera {
private:
	// Look Variables
	double pitch; // deg
	double yaw; // deg
	//vec3 fwd;
	vec3 rgt;
	vec3 up;

	// Look Settings
	double sens;
	double m_yaw = 0.022;
	double m_pitch = 0.022;

	vec3 pos;
	double moveSpeed = 0.01;

	mat4 viewMat;
	void UpdateViewMat();

public:
	void MouseMove(long dx, long dy);
	void MoveDir(vec3 dir, double magnitude);
	void MoveVect(vec3 vect);
	void PrintPos();
	mat4 GetViewMat();
	void SetViewMat(mat4 _viewMat);

	Camera(vec3 _pos, double _pitch, double _yaw, double _sens, double _m_yaw, double _m_pitch);
};