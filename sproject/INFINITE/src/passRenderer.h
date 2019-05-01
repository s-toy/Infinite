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

	void __updateAudioTexture();
	void __updateKeyboardTexture();
	void __updateShaderUniforms4ImagePass();

	void __initAudio();

	void __destroy();

private:
	std::vector<GLuint> m_TextureSet;
	std::string m_AudioFilePath;
	int m_AudioID = -1;

	GLuint m_KeyboardTex = 0;
	GLuint m_RenderTex = 0;
	GLuint m_AudioTex = 0;
	GLuint m_CaptureFBO = 0;
	GLuint m_CaptureRBO = 0;

	CGameShadingTechnique* m_pShadingTechnique = nullptr;
	CSceneRenderer* m_pSceneRenderer = nullptr;
	CMeshRenderer* m_pQuadRenderer = nullptr;

	SPassConfig m_Config;
};