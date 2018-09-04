#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <GLM/glm.hpp>
#include "openglRenderer.h"

class CShadingTechnique;
class CMeshRenderer;
class CPlayer;

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
	void __destroyScene();

	void __initTechniques();
	void __initRenderers();
	void __initTextures();

	void __mainImagePass();
	void __updateShaderUniforms4MainImagePass();

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);

private:
	CShadingTechnique* m_pShadingTechnique;
	CMeshRenderer* m_pQuadRenderer;

	clock_t m_StartTime;
	clock_t m_CurrentTime;

	GLuint m_ChannelTextures[4];
	unsigned int m_CurrentSceneID;
	glm::ivec2 m_WinSize;
};