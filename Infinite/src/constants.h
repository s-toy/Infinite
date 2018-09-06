#pragma once

#include <string>
#include <GLM/glm.hpp>
#include <gl/glew.h>

namespace Constant
{
	static const std::string GAME_CONFIG_FILE_PATH = "gameConfig.json";
	static const std::string DRAW_QUAD_SHADER_PATH = "res/shaders/core/drawQuad_vs.glsl";
	static const std::string MAIN_IMAGE_SHADER_PATH = "res/shaders/core/mainImage_fs.glsl";

	static const float PI = 3.1415926;

	const GLfloat QUAD_VERTICES[] = {
   -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
   -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
   1.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
   -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
   1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   1.0f,  1.0f,  0.0f,  1.0f, 1.0f
	};

	const glm::ivec2 KEYBORAD_TEX_SIZE = { 256, 3 };
}

namespace EChannelType
{
	static const std::string BUFFER = "buffer";
	static const std::string TEXTURE = "texture";
	static const std::string CUBE_MAP = "cubeMap";
	static const std::string KEYBOARD = "keyboard";
	static const std::string SOUND = "sound";
	static const std::string SOUND_CLOUD = "soundCloud";
	static const std::string VIDEO = "video";
}

namespace EPassType
{
	static const std::string BUFFER = "buffer";
	static const std::string IMAGE = "image";
	static const std::string SOUND = "sound";
	static const std::string CUBE_MAP = "cubeMap";
}