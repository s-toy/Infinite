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

	virtual void initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight) override;

protected:
	virtual void _updateV() override;
	virtual void _handleEventsV() override;

private:
	void __initTechniques();
	void __initBuffers();
	void __initRenderers();
	void __initPlayer();

	void __drawBackgroundQuad();
	void __renderBarrage2Texture();
	void __drawBarrageQuad();
	void __drawPlayerQuad();
	bool __detectCollision();
	void __buildCircleSampleOffsets();

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);

private:
	CShadingTechnique* m_pShadingTechnique;
	CMeshRenderer* m_pQuadRenderer;
	CPlayer* m_pPlayer;

	clock_t m_StartTime;
	clock_t m_CurrentTime;
	GLuint m_BarrageTex;
	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;

	std::vector<glm::ivec2> m_CircleSampleOffsets;
};