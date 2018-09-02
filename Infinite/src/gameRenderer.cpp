#include "gameRenderer.h"
#include "gameShadingTechnique.h"
#include "renderUtil.h"
#include "textureUtil.h"
#include "constants.h"

using namespace Constant;

CGameRenderer::CGameRenderer() : m_pShadingTechnique(nullptr)
{

}

CGameRenderer::~CGameRenderer()
{
	__destory();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight)
{
	COpenGLRenderer::initV(vWindowTitle, vWindowWidth, vWindowHeight);
	__initTechniques();
	__initBuffers();

	m_StartTime = clock();
	m_BarrageTex = util::setupTexture(vWindowWidth, vWindowHeight, GL_RGBA32F, GL_RGBA);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::_updateV()
{
	m_CurrentTime = clock();
	__drawBackgroundQuad();
	__renderBarrage2Texture();
	__drawBarrageQuad();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initTechniques()
{
	m_pShadingTechnique = new CGameShadingTechnique();
	m_pShadingTechnique->initTechniqueV();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initBuffers()
{
	glGenFramebuffers(1, &m_CaptureFBO);
	glGenRenderbuffers(1, &m_CaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_CaptureRBO);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__drawBackgroundQuad()
{

}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__renderBarrage2Texture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BarrageTex, 0);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShadingTechnique->enableShader("RenderBarragePass");
	m_pShadingTechnique->updateStandShaderUniform("iResolution", glm::vec2(WIN_WIDTH, WIN_HEIGHT));
	float time = (float)(m_CurrentTime - m_StartTime) / CLOCKS_PER_SEC;
	m_pShadingTechnique->updateStandShaderUniform("iTime", time);

	util::renderScreenQuad();

	m_pShadingTechnique->disableShader();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__drawBarrageQuad()
{
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShadingTechnique->enableShader("TextureCopyPass");
	m_pShadingTechnique->updateStandShaderUniform("uTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_BarrageTex);
	util::renderScreenQuad();
	glBindTexture(GL_TEXTURE_2D, 0);

	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__destory()
{
	if (m_pShadingTechnique) { delete m_pShadingTechnique; m_pShadingTechnique = nullptr; }
}