#include <cstddef>
#include "GLEntityHandler.h"

void GLEntityHandler::SetVBO(GLfloat* data, int size, int vboNum, int dim) {
	if (vboNum == 0) {
		numPoints = size / dim;
	}
	glGenBuffers(1, &vbo[vboNum]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[vboNum]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW);
	dimensions[vboNum] = dim;
}
void GLEntityHandler::SetVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	for (int i = 0; i < numVBOs; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glVertexAttribPointer(i, dimensions[i], GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(i);
	}
}
void GLEntityHandler::LoadTexture(unsigned char* pixels, int width, int height,
                                  int texNum) {
	glGenTextures(1, &texture);
	//glActiveTexture(GL_TEXTURE0 + texNum);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (hasAlpha) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		             GL_UNSIGNED_BYTE, pixels);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		             GL_UNSIGNED_BYTE, pixels);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	                GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
}

void GLEntityHandler::BindAndDraw(GLuint shaderProg, mat4 viewMat, mat4 projMat,
                                  int viewMatLoc, int projMatLoc) {
	glUseProgram(shaderProg);
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, viewMat.m);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, projMat.m);
	glDrawArrays(GL_TRIANGLES, 0, numPoints);
}
void GLEntityHandler::BindAndDraw(GLuint shaderProg) {
	glUseProgram(shaderProg);
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, numPoints);
}

GLEntityHandler::GLEntityHandler(bool _hasAlpha/*= false*/) {
	hasAlpha = _hasAlpha;
}
