#include "textureUtil.h"
#include <iostream>
#include "stb_image.h"

//**********************************************************************************************
//FUNCTION:
GLuint util::loadCubemap(const std::vector<std::string>& vFaces, bool vGenerateMipMap)
{
	GLuint TextureID;
	glGenTextures(1, &TextureID);

	int Width, Height, NrComponents;
	float * pImageData = nullptr;

	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	for (GLuint i = 0; i < vFaces.size(); ++i)
	{
		pImageData = stbi_loadf(vFaces[i].c_str(), &Width, &Height, &NrComponents, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, pImageData);
		stbi_image_free(pImageData);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (vGenerateMipMap)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return TextureID;
}

//**********************************************************************************************
//FUNCTION:
GLuint util::setupCubemap(int vWidth, int vHeight, bool vGenerateMipMap)
{
	GLuint Cubemap;
	glGenTextures(1, &Cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, vWidth, vHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	if (vGenerateMipMap)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return Cubemap;
}

//**********************************************************************************************
//FUNCTION:
GLuint util::loadTexture(const char *vPath)
{
	stbi_set_flip_vertically_on_load(true);
	int Width, Height, Channels;
	GLfloat *pData = stbi_loadf(vPath, &Width, &Height, &Channels, 0);

	if (pData)
	{
		GLenum Format;
		if (Channels == 1)
			Format = GL_RED;
		else if (Channels == 3)
			Format = GL_RGB32F;
		else if (Channels == 4)
			Format = GL_RGBA32F;

		GLuint TextureID;
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		switch (Format)
		{
		case GL_RED:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Width, Height, 0, GL_RED, GL_FLOAT, pData);
			break;
		case GL_RGB32F:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Width, Height, 0, GL_RGB, GL_FLOAT, pData);
			break;
		case GL_RGBA32F:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, pData);
			break;
		default:
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(pData);

		return TextureID;
	}
	else
	{
		std::cout << "Failed to load image." << std::endl;
		return -1;
	}
}

//**********************************************************************************************
//FUNCTION:
GLuint util::setupTexture2D(unsigned int vWidth, unsigned int vHeight, GLint vInternalFormat, GLenum vFormat, GLenum vType, GLint vFilterMode, GLint vWrapMode, bool vGenerateMipMap)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, vInternalFormat, vWidth, vHeight, 0, vFormat, vType, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, vWrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vWrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, vFilterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, vFilterMode);

	if (vGenerateMipMap == true)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}
