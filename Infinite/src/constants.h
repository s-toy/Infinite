#pragma once

#include <string>
#include <GLM/glm.hpp>
#include <gl/glew.h>

namespace Constant
{
	static const std::string WIN_NAME = "Infinite";
	static const int WIN_WIDTH = 1600;
	static const int WIN_HEIGHT = 900;

	const GLfloat QUAD_VERTICES[] = {
   -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
   -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
   1.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
   -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
   1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   1.0f,  1.0f,  0.0f,  1.0f, 1.0f
	};
}