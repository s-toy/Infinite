#include "RenderEngineInterface.h"
#include "DisplayDevice.h"
#include "sceneRenderer.h"
#include "gameConfig.h"
#include "passRenderer.h"
#include "common.h"
#include "constants.h"
#include "gameShadingTechnique.h"
#include "gameRenderer.h"
#include "meshRenderer.h"
#include "textureUtil.h"

CSceneRenderer* CSceneRenderer::m_pInstance = nullptr;

//*********************************************************************************
//FUNCTION:
bool CSceneRenderer::init()
{
	m_pShadingTechnique = CGameShadingTechnique::getInstance();
	auto DisplayInfo = sengine::renderEngine::fetchDisplayDevice()->getDisplayDeviceInfo();
	_ASSERTE(DisplayInfo.WinWidth > 0 && DisplayInfo.WinHeight > 0);
	m_WinSize = glm::ivec2(DisplayInfo.WinWidth, DisplayInfo.WinHeight);

	return true;
}

//*********************************************************************************
//FUNCTION:
void CSceneRenderer::loadScene(int vSceneID)
{
	if (m_CurrentSceneID == vSceneID) return;

	__destroyScene();

	m_CurrentSceneID = vSceneID;
	m_StartTime = clock();
	m_FrameCount = 0;

	const auto& SceneConfigMap = CGameConfig::getInstance()->getConfig().sceneConfigMap;
	if (SceneConfigMap.find(vSceneID) == SceneConfigMap.end()) return;
	m_Config = SceneConfigMap.at(vSceneID);

	m_pShadingTechnique->initV(); //HACK: 这里不需要重新init
	__initRenderTextures();
	__initRenderPasses();
}

//*********************************************************************************
//FUNCTION:
void CSceneRenderer::renderScene()
{
	m_CurrentTime = clock();
	m_FrameCount++;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto e : m_ID2PassRendererMap)
	{
		auto pPassRenderer = e.second;
		pPassRenderer->renderPass();
	}
}

//*********************************************************************************
//FUNCTION:
void CSceneRenderer::__initRenderPasses()
{
	const auto& PassConfigSet = m_Config.passConfigSet;

	for (const auto& PassConfig : PassConfigSet)
	{
		_ASSERTE(PassConfig.isValid());
		auto pPass = new CPassRenderer();
		pPass->init(PassConfig);
		m_ID2PassRendererMap.insert(std::make_pair(PassConfig.passID, pPass));
	}
}

//*********************************************************************************
//FUNCTION:
void CSceneRenderer::__initRenderTextures()
{
	auto pGameRenderer = CGameRenderer::getInstance();
	for (auto& PassConfig : m_Config.passConfigSet)
	{
		auto PassID = PassConfig.passID;
		auto Type = PassConfig.type;
		if (EPassType::BUFFER == Type)
		{
			auto TextureID = util::setupTexture2D(m_WinSize.x, m_WinSize.y, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_BORDER, true);
			m_ID2RenderTextureMap.insert(std::make_pair(PassID, TextureID));
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CSceneRenderer::__destroyScene()
{
	for (auto& e : m_ID2PassRendererMap)
	{
		auto pRenderPass = e.second;
		SAFE_DELETE(pRenderPass);
	}
	m_ID2PassRendererMap.clear();

	m_pShadingTechnique->removeAllProgram();
}