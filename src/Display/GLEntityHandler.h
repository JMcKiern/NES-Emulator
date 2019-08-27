#pragma once

#include <GL/glew.h>
#include "maths_funcs.h"

// Handles a GL Entity - an object that will be rendered
// in the scene.
// Includes the points and texture
// Note: vbo[0] MUST be the points to draw
// (numPoints = size / dim for SetVBO when vboNum = 0)
class GLEntityHandler {
private:
	GLuint vbo[2]; // 0 = points, 1 = texcoords
	int dimensions[2] = {0, 0};
	int numPoints = 0;
	const int POINTS = 0;
	const int TEXCOORDS = 1;
	const int numVBOs = 2;
	bool hasAlpha;

public:
	GLuint vao;
	GLuint texture;

	void SetVBO(GLfloat* array, int size, int vboNum, int dim);
	void SetVAO();
	void LoadTexture(unsigned char* pixels, int width, int height, int texNum);
	void BindAndDraw(GLuint shaderProg, mat4 viewMat, mat4 projMat,
	                 int viewMatLoc, int projMatLoc);
	void BindAndDraw(GLuint shaderProg);

	GLEntityHandler(bool hasAlpha = false);
};
