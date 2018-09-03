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
	void __initTechniques();
	void __initBuffers();
	void __initRenderers();
	void __initPlayer();
	void __initTextures();

	void __renderMainImage2Texture();
	void __mainImagePass();
	void __renderPlayerPass();
	bool __detectCollision();
	void __buildCircleSampleOffsets();
	void __updateShaderUniforms4MainImagePass();

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);

private:
	CShadingTechnique* m_pShadingTechnique;
	CMeshRenderer* m_pQuadRenderer;
	CPlayer* m_pPlayer;

	clock_t m_StartTime;
	clock_t m_CurrentTime;
	GLuint m_MainImageTex;
	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;
	GLuint m_ChannelTextures[4];

	std::vector<glm::ivec2> m_CircleSampleOffsets;
	glm::ivec2 m_WinSize;
};