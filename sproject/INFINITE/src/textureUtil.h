#pragma once

#include <vector>
#include <GL/glew.h>
#include "constants.h"

namespace util
{
	GLuint loadCubemap(const std::vector<std::string>& vFaces, bool vGenerateMipMap);

	GLuint setupCubemap(int vWidth, int vHeight, bool vGenerateMipMap);

	GLuint loadTexture(const char *vPath, GLint vFilterMode = Default::TEXTURE_FILTER_MODE, GLint vWrapMode = Default::TEXTURE_WRAP_MODE, bool vVerticallyFlip = Default::TEXTURE_IS_VFLIP, bool vGenerateMipMap = Default::TEXTURE_IS_MIPMAP);

	GLuint setupTexture2D(unsigned int vWidth, unsigned int vHeight, GLint vInternalFormat = GL_RGB32F, GLenum vFormat = GL_RGB, GLenum vType = GL_FLOAT,
		GLint vFilterMode = GL_LINEAR, GLint vWrapMode = GL_CLAMP_TO_EDGE, bool vGenerateMipMap = GL_FALSE);
}