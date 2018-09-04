#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <GLM/glm.hpp>
#include "openglRenderer.h"

class CShadingTechnique;
class CMeshRenderer;
class CPlayer;
struct SPassConfig;

class CGameRenderer : public COpenGLRenderer
{
public:
	CGameRenderer();
	virtual ~CGameRenderer();

	virtual void initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, int vWinPosX, int vWinPosY, bool vIsFullscreen = false) override;

protected:
	virtual void _updateV() override;
	virtual void _handleEventsV() override;

private:
	void __loadScene(unsigned int vSceneID);
	void __renderScene();
	void __renderPass(int vPassIndex);
	void __destroyScene();

	void __initPasses();
	void __initShaders(const SPassConfig& vPassConfig);
	void __initTextures(const SPassConfig& vPassConfig);
	void __initRenderTextures();
	void __initTechniques();
	void __initRenderers();
	void __initBuffers();

	void __updateShaderUniforms4ImagePass(int vPassIndex);

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);

private:
	CShadingTechnique* m_pShadingTechnique;
	CMeshRenderer* m_pQuadRenderer;

	clock_t m_StartTime;
	clock_t m_CurrentTime;

	std::vector<GLuint> m_RenderTextures;
	std::vector<std::vector<GLuint>> m_ChannelTextures;
	std::vector<SPassConfig> m_PassConfigSet;
	int m_CurrentSceneID;
	glm::ivec2 m_WinSize;

	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;
};