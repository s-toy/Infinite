#pragma once

#include <string>
#include <ctime>
#include <GLM/glm.hpp>
#include "openglRenderer.h"

class CShadingTechnique;

class CGameRenderer : public COpenGLRenderer
{
public:
	CGameRenderer();
	virtual ~CGameRenderer();

	virtual void initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight) override;

protected:
	virtual void _updateV() override;

private:
	void __initTechniques();
	void __initBuffers();

	void __drawBackgroundQuad();
	void __renderBarrage2Texture();
	void __drawBarrageQuad();

	void __destory();

private:
	CShadingTechnique *m_pShadingTechnique;
	clock_t m_StartTime;
	clock_t m_CurrentTime;
	GLuint m_BarrageTex;
	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;
};