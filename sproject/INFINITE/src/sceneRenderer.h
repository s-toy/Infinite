#pragma once

#include <unordered_map>
#include <ctime>
#include <GLM/glm.hpp>
#include <gl/glew.h>
#include "gameConfig.h"

class CPassRenderer;
class CGameShadingTechnique;
class CMeshRenderer;

class CSceneRenderer
{
public:
	virtual ~CSceneRenderer() = default;

	static CSceneRenderer* getInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSceneRenderer();
		return m_pInstance;
	}

	bool init();
	void loadScene(int vSceneID);
	void renderScene();

	const SSceneConfig& getSceneConfig() const { return m_Config; }
	const glm::ivec2& getWinSize() const { return m_WinSize; }
	GLuint getRenderTextureByPassID(int vPassID) const { return m_ID2RenderTextureMap.at(vPassID); }
	float getTime() const { return (float)(m_CurrentTime - m_StartTime) / CLOCKS_PER_SEC; }
	int getFrameCount() const { return m_FrameCount; }
	int getCurrentSceneID() const { return m_CurrentSceneID; }

private:
	CSceneRenderer() = default;

	void __initRenderTextures();
	void __initRenderPasses();

	void __destroyScene();

private:
	static CSceneRenderer* m_pInstance;

	std::unordered_map<int, CPassRenderer*> m_ID2PassRendererMap;
	std::unordered_map<int, GLuint> m_ID2RenderTextureMap;
	SSceneConfig m_Config;

	int m_CurrentSceneID = -1;
	int m_FrameCount = 0;

	clock_t m_StartTime = 0;
	clock_t m_CurrentTime = 0;

	CGameShadingTechnique* m_pShadingTechnique = nullptr;

	glm::ivec2 m_WinSize;
};