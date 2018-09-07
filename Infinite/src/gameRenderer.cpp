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
#include "passRenderer.h"
#include "sceneRenderer.h"

using namespace Constant;

namespace
{
	bool g_Buttons[3];
	bool g_Keys[KEYBORAD_TEX_WIDTH];
	glm::vec2 g_CursorPos;
}

CGameRenderer* CGameRenderer::m_pInstance = nullptr;

CGameRenderer::CGameRenderer() : m_pShadingTechnique(nullptr), m_pSceneRenderer(nullptr)
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

	auto Config = CGameConfig::getInstance()->getConfig();
	m_WinSize = glm::ivec2(Config.winWidth, Config.winHeight);

	glViewport(0, 0, m_WinSize.x, m_WinSize.y);
	glfwSetKeyCallback(m_pWindow, __keyCallback);
	glfwSetMouseButtonCallback(m_pWindow, __mouseButtonCallback);
	glfwSetCursorPosCallback(m_pWindow, __cursorPosCallback);

	m_pShadingTechnique = CGameShadingTechnique::getInstance();
	m_pShadingTechnique->initV();

	m_pSceneRenderer = CSceneRenderer::getInstance();
	m_pSceneRenderer->init();
	m_pSceneRenderer->loadScene(Config.entrySceneID);
}

//*********************************************************************************
//FUNCTION:
bool CGameRenderer::isKeyPressed(int vKeyCode)
{
	return g_Keys[vKeyCode];
}

//*********************************************************************************
//FUNCTION:
bool CGameRenderer::isButtonPressed(int vButtonCode)
{
	return g_Buttons[vButtonCode];
}

//*********************************************************************************
//FUNCTION:
const glm::vec2& CGameRenderer::getCursorPos()
{
	return g_CursorPos;
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::_updateV()
{
	m_pSceneRenderer->renderScene();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::_handleEventsV()
{
	for (int i = 0; i <= 9; ++i) {
		if (g_Keys[GLFW_KEY_0 + i]) m_pSceneRenderer->loadScene(i);
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__destory()
{

}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode)
{
	if (vKey == GLFW_KEY_ESCAPE && vAction == GLFW_PRESS)
		glfwSetWindowShouldClose(vWindow, GL_TRUE);

	if (vKey >= 0 && vKey < KEYBORAD_TEX_WIDTH)
	{
		if (vAction == GLFW_PRESS)
			g_Keys[vKey] = true;
		else if (vAction == GLFW_RELEASE)
			g_Keys[vKey] = false;
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods)
{
	if (vButton >= 0 && vButton < 3)
	{
		if (vAction == GLFW_PRESS)
			g_Buttons[vButton] = true;
		else if (vAction == GLFW_RELEASE)
			g_Buttons[vButton] = false;
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__cursorPosCallback(GLFWwindow* vWindow, double vPosX, double vPosY)
{
	g_CursorPos.x = vPosX;
	g_CursorPos.y = vPosY;
}