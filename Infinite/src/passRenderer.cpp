#include "passRenderer.h"
#include <boost/format.hpp>
#include "constants.h"
#include "common.h"
#include "textureUtil.h"
#include "gameShadingTechnique.h"
#include "sceneRenderer.h"
#include "gameRenderer.h"
#include "meshRenderer.h"

CPassRenderer::CPassRenderer() : m_pShadingTechnique(nullptr), m_pSceneRenderer(nullptr), m_pQuadRenderer(nullptr), m_KeyboardTex(0), m_RenderTex(0)
{

}

CPassRenderer::~CPassRenderer()
{
	__destroy();
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::init(const SPassConfig& vPassConfig)
{
	m_Config = vPassConfig;
	m_pShadingTechnique = CGameShadingTechnique::getInstance();
	m_pSceneRenderer = CSceneRenderer::getInstance();
	m_pQuadRenderer = new CMeshRenderer(Constant::QUAD_VERTICES, sizeof(Constant::QUAD_VERTICES));

	__initTextures();
	__initBuffers();
	__initShaders();
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::renderPass()
{
	auto& PassID = m_Config.passID;
	auto& Type = m_Config.type;

	if (EPassType::BUFFER == Type) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	}
	else if (EPassType::IMAGE == Type) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	auto PassName = boost::str(boost::format("pass%1%") % PassID);

	__updateKeyboardTexture();

	m_pShadingTechnique->enableProgram(PassName);
	__updateShaderUniforms4ImagePass();
	m_pQuadRenderer->draw();
	m_pShadingTechnique->disableProgram();

	for (int i = 0; i < m_TextureSet.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (EPassType::BUFFER == Type) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::__initTextures()
{
	for (auto& Channel : m_Config.channels)
	{
		auto ChannelType = Channel.first;
		auto ChannelValue = Channel.second;

		if (EChannelType::BUFFER == ChannelType) {
			auto Index = atoi(ChannelValue.c_str());
			auto TextureID = m_pSceneRenderer->getRenderTextureByPassID(Index);
			_ASSERTE(TextureID > 0);
			m_TextureSet.push_back(TextureID);
		}
		else if (EChannelType::TEXTURE == ChannelType) {
			auto TextureID = util::loadTexture(ChannelValue.c_str());
			_ASSERTE(TextureID > 0);
			m_TextureSet.push_back(TextureID);
		}
		else if (EChannelType::KEYBOARD == ChannelType) {
			m_KeyboardTex = util::setupTexture2D(KEYBORAD_TEX_WIDTH, KEYBORAD_TEX_HEIGHT, GL_RED, GL_RED, GL_UNSIGNED_BYTE, GL_NEAREST);
			_ASSERTE(m_KeyboardTex > 0);
			m_TextureSet.push_back(m_KeyboardTex);
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::__initShaders()
{
	auto pShadingPass = new CProgram;
	const SSceneConfig& SceneConfig = CSceneRenderer::getInstance()->getSceneConfig();

	pShadingPass->addShader(Constant::DRAW_QUAD_SHADER_PATH, VERTEX_SHADER);
	std::vector<std::string> ShaderFilesPaths = { Constant::MAIN_IMAGE_SHADER_PATH };
	if (!SceneConfig.commonShaderPath.empty()) { ShaderFilesPaths.push_back(SceneConfig.commonShaderPath); }
	ShaderFilesPaths.push_back(m_Config.shaderPath);

	pShadingPass->addShader(ShaderFilesPaths, FRAGMENT_SHADER);
	auto PassName = boost::str(boost::format("pass%1%") % m_Config.passID);
	m_pShadingTechnique->addProgram(PassName, pShadingPass);
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::__initBuffers()
{
	if (EPassType::BUFFER != m_Config.type) return;

	glGenFramebuffers(1, &m_CaptureFBO);
	glGenRenderbuffers(1, &m_CaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_CaptureRBO);

	auto WinSize = m_pSceneRenderer->getWinSize();
	m_RenderTex = m_pSceneRenderer->getRenderTextureByPassID(m_Config.passID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WinSize.x, WinSize.y);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTex, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::__updateKeyboardTexture()
{
	if (0 == m_KeyboardTex) return;

	const int Width = KEYBORAD_TEX_WIDTH, Height = KEYBORAD_TEX_HEIGHT;
	GLubyte ImageData[Width*Height] = { 0 };
	for (int i = 0; i < Width; ++i) {
		if (CGameRenderer::isKeyPressed(i)) {
			ImageData[i] = 0xff;
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_KeyboardTex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, GL_RED, GL_UNSIGNED_BYTE, ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::__updateShaderUniforms4ImagePass()
{
	m_pShadingTechnique->updateStandShaderUniform("iResolution", glm::vec2(m_pSceneRenderer->getWinSize()));
	m_pShadingTechnique->updateStandShaderUniform("iTime", m_pSceneRenderer->getTime());
	m_pShadingTechnique->updateStandShaderUniform("iTimeDelta", CGameRenderer::getInstance()->getDeltaTime());
	m_pShadingTechnique->updateStandShaderUniform("iFrame", m_pSceneRenderer->getFrameCount());

	for (int i = 0; i < m_TextureSet.size(); ++i) {
		auto TextureID = m_TextureSet[i];
		_ASSERTE(TextureID != 0);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		std::string UniformName = boost::str(boost::format("iChannel%1%") % i);
		m_pShadingTechnique->updateStandShaderUniform(UniformName, i);

		GLint Width, Height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &Width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &Height);
		UniformName = boost::str(boost::format("iChannelResolution[%1%]") % i);
		m_pShadingTechnique->updateStandShaderUniform(UniformName, glm::vec3(Width, Height, 0));
	}

	glm::vec2 MousePos = glm::vec2(0);
	bool IsButton1Pressed = CGameRenderer::isButtonPressed(GLFW_MOUSE_BUTTON_1);
	bool IsButton2Pressed = CGameRenderer::isButtonPressed(GLFW_MOUSE_BUTTON_2);
	if (IsButton1Pressed || IsButton2Pressed)
	{
		auto Pos = CGameRenderer::getCursorPos();
		auto WinSize = m_pSceneRenderer->getWinSize();
		MousePos = glm::vec2(Pos.x, WinSize.y - Pos.y);
	}
	glm::vec4 Mouse = glm::vec4(MousePos.x, MousePos.y, IsButton1Pressed - 0.5f, IsButton2Pressed - 0.5f);
	m_pShadingTechnique->updateStandShaderUniform("iMouse", Mouse);
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::__destroy()
{
	SAFE_DELETE(m_pQuadRenderer);

	for each (auto Tex in m_TextureSet)
	{
		glDeleteTextures(1, &Tex);
	}
	glDeleteTextures(1, &m_KeyboardTex);
	glDeleteTextures(1, &m_RenderTex);

	glDeleteBuffers(1, &m_CaptureRBO);
	glDeleteBuffers(1, &m_CaptureFBO);
}
