#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <GLM/glm.hpp>
#include "openglRenderer.h"

class CSceneRenderer;
class CShadingTechnique;
class CMeshRenderer;
class CPassRenderer;
struct SPassConfig;

class CGameRenderer : public COpenGLRenderer
{
public:
	virtual ~CGameRenderer();

	static CGameRenderer* getInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGameRenderer();
		return m_pInstance;
	}

	virtual void initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, int vWinPosX, int vWinPosY, bool vIsFullscreen = false) override;

	const glm::ivec2& getWinSize() const { return m_WinSize; }
	float getDeltaTime() const { return m_DeltaTime; }

	static bool isKeyPressed(int vKeyCode);
	static bool isButtonPressed(int vButtonCode);
	static const glm::vec2& getCursorPos();

protected:
	virtual void _updateV() override;
	virtual void _handleEventsV() override;

private:
	CGameRenderer();

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);
	static void __cursorPosCallback(GLFWwindow* vWindow, double vPosX, double vPosY);
	static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);

private:
	static CGameRenderer* m_pInstance;

	CShadingTechnique* m_pShadingTechnique;
	CSceneRenderer* m_pSceneRenderer;

	glm::ivec2 m_WinSize;
};