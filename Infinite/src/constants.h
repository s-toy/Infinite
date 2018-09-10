#pragma once

#include <string>
#include <GLM/glm.hpp>
#include <gl/glew.h>

namespace Constant
{
	static const std::string GAME_CONFIG_FILE_PATH = "gameConfig.json";
	static const std::string DRAW_QUAD_SHADER_PATH = "res/shaders/core/drawQuad_vs.glsl";
	static const std::string MAIN_IMAGE_SHADER_PATH = "res/shaders/core/mainImage_fs.glsl";

	static const float PI = 3.1415926f;
	static const int KEYBORAD_TEX_WIDTH = 512;
	static const int KEYBORAD_TEX_HEIGHT = 3;

	const GLfloat QUAD_VERTICES[] = {
   -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
   -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
   1.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
   -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
   1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   1.0f,  1.0f,  0.0f,  1.0f, 1.0f
	};
}

namespace Default
{
	const GLint TEXTURE_FILTER_MODE = GL_LINEAR;
	const GLint TEXTURE_WRAP_MODE = GL_CLAMP_TO_EDGE;
	const bool	TEXTURE_IS_VFLIP = true;
}