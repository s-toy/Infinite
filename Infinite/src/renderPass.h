#pragma once

#include <gl/glew.h>
#include "gameConfig.h"

class CGameShadingTechnique;

class CRenderPass
{
public:
	CRenderPass();
	virtual ~CRenderPass();

	void init(const SPassConfig& vPassConfig);

private:
	void __initTextures();
	void __initShaders();

	void __destroy();

private:
	SPassConfig m_Config;

	std::vector<GLuint> m_TextureSet;
	GLuint m_RenderTexture;

	CGameShadingTechnique* m_pShadingTechnique;
};