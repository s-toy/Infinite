#include "gameRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <boost/format.hpp>
#include "gameShadingTechnique.h"
#include "meshRenderer.h"
#include "textureUtil.h"
#include "constants.h"
#include "common.h"
#include "gameConfig.h"

using namespace Constant;

namespace
{
	bool g_Keys[1024];
}

CGameRenderer::CGameRenderer() : m_pShadingTechnique(nullptr), m_pQuadRenderer(nullptr), m_CurrentSceneID(0)
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
	__initRenderers();

	auto Config = CGameConfig::getInstance()->getConfig();
	m_WinSize = glm::ivec2(Config.winWidth, Config.winHeight);

	glViewport(0, 0, m_WinSize.x, m_WinSize.y);
	glfwSetKeyCallback(m_pWindow, __keyCallback);

	__loadScene(Config.entrySceneID);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::_updateV()
{
	m_CurrentTime = clock();
	__renderScene();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::_handleEventsV()
{
	if (g_Keys[GLFW_KEY_0])
		__loadScene(0);
	if (g_Keys[GLFW_KEY_1])
		__loadScene(1);
	if (g_Keys[GLFW_KEY_2])
		__loadScene(2);
	if (g_Keys[GLFW_KEY_3])
		__loadScene(3);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__loadScene(unsigned int vSceneID)
{
	if (m_CurrentSceneID == vSceneID) return;

	m_CurrentSceneID = vSceneID;
	m_StartTime = clock();

	__initTextures();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__renderScene()
{
	__mainImagePass();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__destroyScene()
{

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
void CGameRenderer::__initRenderers()
{
	m_pQuadRenderer = new CMeshRenderer(QUAD_VERTICES, sizeof(QUAD_VERTICES));
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initTextures()
{
	//TODO: release texture.
	auto SceneConfig = CGameConfig::getInstance()->getConfig().sceneConfigMap.at(m_CurrentSceneID);
	for (int i = 0; i < 4; ++i)
	{
		if (!SceneConfig.iChannel[i].empty())
			m_ChannelTextures[i] = util::loadTexture(SceneConfig.iChannel[i].c_str());
		else
			m_ChannelTextures[i] = 0;
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__mainImagePass()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto MainImagePassName = boost::str(boost::format("mainImagePass%1%") % m_CurrentSceneID);
	m_pShadingTechnique->enableShader(MainImagePassName);

	__updateShaderUniforms4MainImagePass();
	m_pQuadRenderer->draw();

	m_pShadingTechnique->disableShader();

	for (int i = 0; i < 4; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
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
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_ChannelTextures[i]);
			std::string UniformName = boost::str(boost::format("iChannel%1%") % i);
			m_pShadingTechnique->updateStandShaderUniform(UniformName, i);
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__destory()
{
	SAFE_DELETE(m_pShadingTechnique);
	SAFE_DELETE(m_pQuadRenderer);
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