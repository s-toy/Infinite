#include "gameRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <boost/format.hpp>
#include "gameShadingTechnique.h"
#include "meshRenderer.h"
#include "textureUtil.h"
#include "constants.h"
#include "common.h"
#include "player.h"
#include "gameConfig.h"

using namespace Constant;

namespace
{
	bool g_Keys[1024];
	const int CIRCLE_SAMPLE_COUNT = 64;
	const float INTERSECT_THRESHOLD = 0.2f;
}

CGameRenderer::CGameRenderer() : m_pShadingTechnique(nullptr), m_pQuadRenderer(nullptr), m_pPlayer(nullptr)
{

}

CGameRenderer::~CGameRenderer()
{
	__destory();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, int vWinPosX, int vWinPosY, bool vIsFullscreen)
{
	COpenGLRenderer::initV(vWindowTitle, vWindowWidth, vWindowHeight, vWinPosX, vWinPosY, vIsFullscreen);
	__initTechniques();
	__initBuffers();
	__initTextures();
	__initRenderers();
	__initPlayer();
	__buildCircleSampleOffsets();

	m_StartTime = clock();
	m_MainImageTex = util::setupTexture(vWindowWidth, vWindowHeight, GL_RGBA32F, GL_RGBA);
	auto Config = CGameConfig::getInstance()->getConfig();
	m_WinSize = glm::ivec2(Config.winWidth, Config.winHeight);

	glViewport(0, 0, m_WinSize.x, m_WinSize.y);
	glfwSetKeyCallback(m_pWindow, __keyCallback);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::_updateV()
{
	m_CurrentTime = clock();
	__mainImagePass();
	__renderPlayerPass();

	bool IsIntersected = __detectCollision();
	//static int i = 0;
	//if (IsIntersected) std::cout << "intersect:" << ++i << std::endl;
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::_handleEventsV()
{
	if (g_Keys[GLFW_KEY_UP])
		m_pPlayer->moveUp();
	else if (g_Keys[GLFW_KEY_DOWN])
		m_pPlayer->moveDown();

	if (g_Keys[GLFW_KEY_LEFT])
		m_pPlayer->moveLeft();
	else if (g_Keys[GLFW_KEY_RIGHT])
		m_pPlayer->moveRight();
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
void CGameRenderer::__initRenderers()
{
	m_pQuadRenderer = new CMeshRenderer(QUAD_VERTICES, sizeof(QUAD_VERTICES));
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initPlayer()
{
	m_pPlayer = new CPlayer();
	m_pPlayer->setPosition(glm::vec2(800.0f, 100.0f));
	m_pPlayer->setSpeed(0.2f);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initTextures()
{
	auto Config = CGameConfig::getInstance()->getConfig();
	for (int i = 0; i < 4; ++i)
	{
		if (!Config.iChannel[i].empty())
			m_ChannelTextures[i] = util::loadTexture(Config.iChannel[i].c_str());
		else
			m_ChannelTextures[i] = 0;
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__renderMainImage2Texture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_WinSize.x, m_WinSize.y);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MainImageTex, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShadingTechnique->enableShader("mainImagePass");

	__updateShaderUniforms4MainImagePass();

	m_pQuadRenderer->draw();
	m_pShadingTechnique->disableShader();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__mainImagePass()
{
	__renderMainImage2Texture();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShadingTechnique->enableShader("textureCopyPass");
	m_pShadingTechnique->updateStandShaderUniform("uTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_MainImageTex);
	m_pQuadRenderer->draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__renderPlayerPass()
{
	m_pShadingTechnique->enableShader("renderPlayerPass");

	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	auto Pos = 2.0f * m_pPlayer->position() / glm::vec2(m_WinSize) - 1.0f;
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(Pos, 0.0f));
	float Scale = 0.1f;
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(Scale));
	m_pShadingTechnique->updateStandShaderUniform("uModelMatrix", ModelMatrix);
	m_pShadingTechnique->updateStandShaderUniform("iResolution", Scale * glm::vec2(m_WinSize));
	m_pShadingTechnique->updateStandShaderUniform("iRadius", (float)m_pPlayer->radius());

	m_pQuadRenderer->draw();

	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
bool CGameRenderer::__detectCollision()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_WinSize.x, m_WinSize.y);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MainImageTex, 0);

	auto Pos = m_pPlayer->position();
	auto Radius = m_pPlayer->radius();
	bool IsIntersected = false;

	int W = 2 * Radius + 1;
	float *pData = new float[4 * W * W];
	glReadPixels(Pos.x - Radius, Pos.y - Radius, W, W, GL_RGBA, GL_FLOAT, pData);
	for (auto Offset : m_CircleSampleOffsets)
	{
		int SamplePos = 4 * (Offset.y * W + Offset.x);
		float *pRGBA = &pData[SamplePos];

		float H = INTERSECT_THRESHOLD;
		if (pRGBA[0] > H || pRGBA[1] > H || pRGBA[2] > H) { IsIntersected = true; break; }
	}

	SAFE_DELETE_ARRAY(pData);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return IsIntersected;
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__buildCircleSampleOffsets()
{
	float Theta = 0.0f;
	const float Delta = 2 * PI / CIRCLE_SAMPLE_COUNT;
	for (float Theta = 0.0f; Theta < 2 * PI; Theta += Delta)
	{
		int R = m_pPlayer->radius();
		int X = cos(Theta) * R + R;
		int Y = sin(Theta) * R + R;

		m_CircleSampleOffsets.emplace_back(glm::ivec2(X, Y));
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__updateShaderUniforms4MainImagePass()
{
	m_pShadingTechnique->updateStandShaderUniform("iResolution", glm::vec2(m_WinSize));
	float Time = (float)(m_CurrentTime - m_StartTime) / CLOCKS_PER_SEC;
	m_pShadingTechnique->updateStandShaderUniform("iTime", Time);

	for (int i = 0; i < 4; ++i) {
		if (m_ChannelTextures[i] != 0) {
			std::string UniformName = boost::str(boost::format("iChannel%1%") % i);
			m_pShadingTechnique->updateStandShaderUniform(UniformName, m_ChannelTextures[i]);
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__destory()
{
	SAFE_DELETE(m_pShadingTechnique);
	SAFE_DELETE(m_pQuadRenderer);
	SAFE_DELETE(m_pPlayer);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode)
{
	if (vKey == GLFW_KEY_ESCAPE && vAction == GLFW_PRESS)
		glfwSetWindowShouldClose(vWindow, GL_TRUE);
	if (vKey >= 0 && vKey < 1024)
	{
		if (vAction == GLFW_PRESS)
			g_Keys[vKey] = true;
		else if (vAction == GLFW_RELEASE)
			g_Keys[vKey] = false;
	}
}