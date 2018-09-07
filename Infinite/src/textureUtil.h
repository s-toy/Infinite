#pragma once

#include <vector>
#include <GL/glew.h>

namespace util
{
	GLuint loadCubemap(const std::vector<std::string>& vFaces, bool vGenerateMipMap);

	GLuint setupCubemap(int vWidth, int vHeight, bool vGenerateMipMap);

	GLuint loadTexture(const char *vPath);

	GLuint setupTexture2D(unsigned int vWidth, unsigned int vHeight, GLint vInternalFormat = GL_RGB32F, GLenum vFormat = GL_RGB, GLenum vType = GL_FLOAT,
		GLint vFilterMode = GL_LINEAR, GLint vWrapMode = GL_CLAMP_TO_EDGE, bool vGenerateMipMap = GL_FALSE);
}