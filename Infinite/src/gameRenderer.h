#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <GLM/glm.hpp>
#include "openglRenderer.h"

class CSceneManager;
class CShadingTechnique;
class CMeshRenderer;
class CRenderPass;
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
	void __renderPass(int vPassIndex);

	void __initRenderTextures();
	void __initRenderers();
	void __initBuffers();

	void __updateShaderUniforms4ImagePass(int vPassIndex);

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);
	static void __cursorPosCallback(GLFWwindow* vWindow, double vPosX, double vPosY);
	static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);

private:
	CShadingTechnique* m_pShadingTechnique;
	CSceneManager* m_pSceneManager;

	CMeshRenderer* m_pQuadRenderer;

	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;
	GLuint m_KeyboardTex;

	glm::ivec2 m_WinSize;
};