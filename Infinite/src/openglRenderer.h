#pragma once

#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

class COpenGLRenderer
{
public:
	COpenGLRenderer();
	virtual ~COpenGLRenderer();

	virtual void initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, int vWinPosX, int vWinPosY, bool vIsFullscreen = false);
	virtual int runV();

protected:
	virtual void _updateV();
	virtual void _handleEventsV();

protected:
	GLFWwindow *m_pWindow;
	float m_DeltaTime;
	float m_LastFrame;
};