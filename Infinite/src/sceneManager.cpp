#include "sceneManager.h"
#include "gameConfig.h"
#include "renderPass.h"
#include "common.h"
#include "gameShadingTechnique.h"

CSceneManager* CSceneManager::m_pInstance = nullptr;

CSceneManager::CSceneManager() : m_CurrentSceneID(-1), m_FrameCount(0)
{
	m_pShadingTechnique = CGameShadingTechnique::getInstance();
}

CSceneManager::~CSceneManager()
{

}

//*********************************************************************************
//FUNCTION:
void CSceneManager::initScene(int vSceneID)
{
	if (m_CurrentSceneID == vSceneID) return;

	__destroyScene();

	m_CurrentSceneID = vSceneID;
	m_StartTime = clock();
	m_FrameCount = 0;

	const auto& SceneConfigMap = CGameConfig::getInstance()->getConfig().sceneConfigMap;
	if (SceneConfigMap.find(vSceneID) == SceneConfigMap.end()) return;
	m_Config = SceneConfigMap.at(vSceneID);

	__initRenderPasses();
}

//*********************************************************************************
//FUNCTION:
void CSceneManager::renderScene()
{
	m_CurrentTime = clock();
	m_FrameCount++;
}

//*********************************************************************************
//FUNCTION:
void CSceneManager::__initRenderPasses()
{
	const auto& PassConfigSet = m_Config.passConfigSet;

	for (const auto& PassConfig : PassConfigSet)
	{
		_ASSERTE(PassConfig.isValid());
		auto pPass = new CRenderPass();
		pPass->init(PassConfig);
		m_ID2RenderPassMap.insert(std::make_pair(PassConfig.passID, pPass));
	}
}

//*********************************************************************************
//FUNCTION:
void CSceneManager::__destroyScene()
{
	for (auto& e : m_ID2RenderPassMap)
	{
		auto pRenderPass = e.second;
		SAFE_DELETE(pRenderPass);
	}
	m_ID2RenderPassMap.clear();

	m_pShadingTechnique->removeAllProgram();
}
