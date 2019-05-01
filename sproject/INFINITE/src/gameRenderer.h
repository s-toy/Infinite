#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <GLM/glm.hpp>
#include "GLRenderer.h"

class CSceneRenderer;
class CShadingTechnique;
class CMeshRenderer;
class CPassRenderer;
struct SPassConfig;

class CGameRenderer : public sengine::renderEngine::CGLRenderer
{
public:
	CGameRenderer();
	virtual ~CGameRenderer();

	static CGameRenderer* getInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGameRenderer();
		return m_pInstance;
	}

	virtual bool _initV() override;

	static bool isKeyPressed(int vKeyCode);
	static bool isButtonPressed(int vButtonCode);
	static const glm::vec2& getCursorPos();

protected:
	virtual bool _renderV() override;
	virtual void _handleEventV() override;

private:
	static CGameRenderer* m_pInstance;
	CShadingTechnique* m_pShadingTechnique = nullptr;
	CSceneRenderer* m_pSceneRenderer = nullptr;
	glm::ivec2 m_WinSize;

	void __destory();
	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);
	static void __cursorPosCallback(GLFWwindow* vWindow, double vPosX, double vPosY);
	static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
};