#pragma once

#include <unordered_map>
#include <ctime>
#include "gameConfig.h"

class CRenderPass;
class CGameShadingTechnique;

class CSceneManager
{
public:
	virtual ~CSceneManager();

	static CSceneManager* getInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSceneManager();
		return m_pInstance;
	}

	void initScene(int vSceneID);
	void renderScene();

	int getCurrentSceneID() const { return m_CurrentSceneID; }

private:
	CSceneManager();

	void __initRenderPasses();

	void __destroyScene();

private:
	static CSceneManager* m_pInstance;

	std::unordered_map<int, CRenderPass*> m_ID2RenderPassMap;
	SSceneConfig m_Config;

	int m_CurrentSceneID;
	int m_FrameCount;

	clock_t m_StartTime;
	clock_t m_CurrentTime;

	CGameShadingTechnique* m_pShadingTechnique;
};