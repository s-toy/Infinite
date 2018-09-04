#pragma once

#include <string>
#include <GLM/glm.hpp>
#include <gl/glew.h>

namespace Constant
{
	static const std::string GAME_CONFIG_FILE_NAME = "gameConfig.json";

	static const float PI = 3.1415926;

	const GLfloat QUAD_VERTICES[] = {
   -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
   -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
   1.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
   -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
   1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   1.0f,  1.0f,  0.0f,  1.0f, 1.0f
	};
}

namespace ChannelType
{
	static const std::string BUFFER = "buffer";
	static const std::string TEXTURE = "texture";
}

namespace PassType
{
	static const std::string BUFFER = "buffer";
	static const std::string IMAGE = "image";
}