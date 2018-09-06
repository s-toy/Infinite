#include "passRenderer.h"
#include <boost/format.hpp>
#include "constants.h"
#include "common.h"
#include "textureUtil.h"
#include "gameShadingTechnique.h"
#include "sceneRenderer.h"
#include "gameRenderer.h"
#include "meshRenderer.h"

CPassRenderer::CPassRenderer() : m_pShadingTechnique(nullptr), m_pSceneRenderer(nullptr), m_pQuadRenderer(nullptr), m_KeyboardTex(0)
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

	__initBuffers();
	__initTextures();
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
		auto WinSize = m_pSceneRenderer->getWinSize();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WinSize.x, WinSize.y);
		auto TextureID = m_pSceneRenderer->getRenderTexture(PassID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);
	}

	if (EPassType::IMAGE == Type) {
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
			auto TextureID = m_pSceneRenderer->getRenderTexture(Index);
			_ASSERTE(TextureID > 0);
			m_TextureSet.push_back(TextureID);
		}
		else if (EChannelType::TEXTURE == ChannelType) {
			auto TextureID = util::loadTexture(ChannelValue.c_str());
			_ASSERTE(TextureID > 0);
			m_TextureSet.push_back(TextureID);
		}
		else if (EChannelType::KEYBOARD == ChannelType) {
			auto TexSize = Constant::KEYBORAD_TEX_SIZE;
			m_KeyboardTex = util::setupTexture(TexSize.x, TexSize.y, GL_RED, GL_RED);
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
void CPassRenderer::__updateKeyboardTexture()
{
	if (0 == m_KeyboardTex) return;

	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	const auto& TexSize = Constant::KEYBORAD_TEX_SIZE;
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, TexSize.x, TexSize.y);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_KeyboardTex, 0);

	std::string ProgramSig = "renderKeyEvent2TexPass";
	m_pShadingTechnique->enableProgram(ProgramSig);

	const int ArraySize = 256 * 3;
	GLfloat KeyState[ArraySize] = { 0.0 };
	for (int i = 0; i < TexSize.x; ++i) {
		if (CGameRenderer::isKeyPressed(i)) KeyState[i * 3] = 1.0;
	}

	GLint Loc = glGetUniformLocation(m_pShadingTechnique->getProgramID(ProgramSig), "uKeyState");
	glUniform1fv(Loc, ArraySize, KeyState);
	m_pQuadRenderer->draw();
	m_pShadingTechnique->disableProgram();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CPassRenderer::__updateShaderUniforms4ImagePass()
{
	m_pShadingTechnique->updateStandShaderUniform("iResolution", glm::vec2(m_pSceneRenderer->getWinSize()));
	m_pShadingTechnique->updateStandShaderUniform("iTime", m_pSceneRenderer->getTime());
	m_pShadingTechnique->updateStandShaderUniform("iFrame", m_pSceneRenderer->getFrameCount());

	for (int i = 0; i < m_TextureSet.size(); ++i) {
		auto TextureID = m_TextureSet[i];
		_ASSERTE(TextureID != 0);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		std::string UniformName = boost::str(boost::format("iChannel%1%") % i);
		m_pShadingTechnique->updateStandShaderUniform(UniformName, i);
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
}
