#pragma once

#include <gl/glew.h>
#include "gameConfig.h"

class CGameShadingTechnique;
class CSceneRenderer;
class CMeshRenderer;

class CPassRenderer
{
public:
	CPassRenderer();
	virtual ~CPassRenderer();

	void init(const SPassConfig& vPassConfig);
	void renderPass();

private:
	void __initTextures();
	void __initShaders();
	void __initBuffers();

	void __updateKeyboardTexture();
	void __updateShaderUniforms4ImagePass();

	void __destroy();

private:
	SPassConfig m_Config;

	std::vector<GLuint> m_TextureSet;
	GLuint m_KeyboardTex;

	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;

	CGameShadingTechnique* m_pShadingTechnique;
	CSceneRenderer* m_pSceneRenderer;
	CMeshRenderer* m_pQuadRenderer;
};