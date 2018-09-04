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

CGameRenderer::CGameRenderer() : m_pShadingTechnique(nullptr), m_pQuadRenderer(nullptr), m_CurrentSceneID(-1)
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
	__initBuffers();

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
	for (int i = 0; i <= 9; ++i) {
		if (g_Keys[GLFW_KEY_0 + i]) __loadScene(i);
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__loadScene(unsigned int vSceneID)
{
	if (m_CurrentSceneID == vSceneID) return;

	m_CurrentSceneID = vSceneID;
	m_StartTime = clock();

	__initPasses();
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__renderScene()
{
	for (int i = 0; i < m_PassConfigSet.size(); ++i)
	{
		__renderPass(i);
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__renderPass(int vPassIndex)
{
	auto& PassConfig = m_PassConfigSet[vPassIndex];
	auto& Type = PassConfig.type;

	if (PassType::BUFFER == Type) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_WinSize.x, m_WinSize.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTextures[vPassIndex], 0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto PassName = boost::str(boost::format("pass%1%") % PassConfig.passID);
	m_pShadingTechnique->enableProgram(PassName);

	__updateShaderUniforms4ImagePass(vPassIndex);
	m_pQuadRenderer->draw();

	m_pShadingTechnique->disableProgram();

	for (int i = 0; i < m_ChannelTextures[vPassIndex].size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (PassType::BUFFER == Type) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__destroyScene()
{

}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initPasses()
{
	const SGameConfig& GameConfig = CGameConfig::getInstance()->getConfig();
	const SSceneConfig& SceneConfig = GameConfig.sceneConfigMap.at(m_CurrentSceneID);

	m_PassConfigSet = SceneConfig.passConfigSet;

	m_ChannelTextures.clear();
	m_RenderTextures.clear();
	m_pShadingTechnique->removeAllProgram();

	__initRenderTextures();

	for (const SPassConfig& PassConfig : m_PassConfigSet)
	{
		__initShaders(PassConfig);
		__initTextures(PassConfig);
	}
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initShaders(const SPassConfig& vPassConfig)
{
	auto pShadingPass = new CProgram;
	pShadingPass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	std::vector<std::string> ShaderFilesPaths = { "res/shaders/core/mainImage_fs.glsl", vPassConfig.shaderPath };
	pShadingPass->addShader(ShaderFilesPaths, FRAGMENT_SHADER);
	auto PassName = boost::str(boost::format("pass%1%") % vPassConfig.passID);
	m_pShadingTechnique->addProgram(PassName, pShadingPass);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initTextures(const SPassConfig& vPassConfig)
{
	std::vector<GLuint> Textures;
	for (auto& Channel : vPassConfig.channels)
	{
		if (Channel.first.empty()) continue;
		auto Type = Channel.first;
		auto Value = Channel.second;

		if (ChannelType::BUFFER == Type)
		{
			auto Index = atoi(Value.c_str());
			Textures.push_back(m_RenderTextures[Index]);
		}
		else
		{
			auto Texture = util::loadTexture(Value.c_str());
			Textures.push_back(Texture);
		}
	}

	m_ChannelTextures.push_back(Textures);
}

//*********************************************************************************
//FUNCTION:
void CGameRenderer::__initRenderTextures()
{
	for (const SPassConfig& PassConfig : m_PassConfigSet)
	{
		auto Type = PassConfig.type;
		if (PassType::BUFFER == Type)
		{
			auto Texture = util::setupTexture(m_WinSize.x, m_WinSize.y, GL_RGBA32F, GL_RGBA);
			m_RenderTextures.push_back(Texture);
		}
	}
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
void CGameRenderer::__updateShaderUniforms4ImagePass(int vPassIndex)
{
	m_pShadingTechnique->updateStandShaderUniform("iResolution", glm::vec2(m_WinSize));
	float Time = (float)(m_CurrentTime - m_StartTime) / CLOCKS_PER_SEC;
	m_pShadingTechnique->updateStandShaderUniform("iTime", Time);

	for (int i = 0; i < m_ChannelTextures[vPassIndex].size(); ++i) {
		auto Texture = m_ChannelTextures[vPassIndex][i];
		if (Texture != 0) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, Texture);
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