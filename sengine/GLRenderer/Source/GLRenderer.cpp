#include "GLRenderer.h"
#include <common/HiveCommonMicro.h>
#include <common/CommonInterface.h>
#include <common/ProductFactory.h>
#include "RenderEngineInterface.h"
#include "DisplayDevice.h"

using namespace sengine::renderEngine;

hiveOO::CProductFactory<CGLRenderer> Creator("GLRenderer");

CGLRenderer::CGLRenderer()
{

}

CGLRenderer::~CGLRenderer()
{

}

//*********************************************************************************
//FUNCTION:
bool CGLRenderer::_initV()
{
	_HIVE_EARLY_RETURN(!__createGLFWWindow(), "Fail to initialize opengl render engine due to failure of __createGLFWWindow().", false);
	hiveCommon::hiveOutputEvent("Succeed to init OpenGL renderer.");

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CGLRenderer::_renderV()
{
	_handleEventV();
	__updateFrameInterval();
	glfwPollEvents();
	glfwSwapBuffers(m_pWindow);

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CGLRenderer::_isRenderLoopDoneV()
{
	bool IsRenderLoopDone = glfwWindowShouldClose(m_pWindow);
	if (IsRenderLoopDone) glfwTerminate();

	return IsRenderLoopDone;
}

//*********************************************************************************
//FUNCTION:
bool CGLRenderer::__createGLFWWindow()
{
	_HIVE_EARLY_RETURN(!glfwInit(), "Fail to initialize glfw due to failure of glfwInit().", false);

	auto pDisplayDevice = fetchDisplayDevice();
	_ASSERTE(pDisplayDevice);
	auto DisplayInfo = pDisplayDevice->getDisplayDeviceInfo();
	_ASSERTE(DisplayInfo.isValid());

	glfwWindowHint(GLFW_RESIZABLE, DisplayInfo.IsWindowResizable);

	_ASSERTE(!m_pWindow);
	GLFWmonitor *pMonitor = DisplayInfo.IsFullScreenWindow ? glfwGetPrimaryMonitor() : nullptr;
	m_pWindow = glfwCreateWindow(DisplayInfo.WinWidth, DisplayInfo.WinHeight, DisplayInfo.WinTitle.c_str(), pMonitor, nullptr);
	if (!m_pWindow)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Fail to create window due to failure of glfwCreateWindow()");
		glfwTerminate();
		return false;
	}

	glfwSetWindowPos(m_pWindow, DisplayInfo.WinPosX, DisplayInfo.WinPosY);
	glfwMakeContextCurrent(m_pWindow);
	glewExperimental = GL_TRUE;

	_HIVE_EARLY_RETURN((glewInit() != GLEW_OK), "Fail to initialize glew due to failure of glewInit().", false);

	return true;
}

//*********************************************************************************
//FUNCTION:
void CGLRenderer::_handleEventV()
{

}

//*********************************************************************************
//FUNCTION:
void CGLRenderer::__updateFrameInterval()
{
	double CurrentFrameTime = static_cast<GLfloat>(glfwGetTime());
	static double LastFrameTime = 0.0;
	m_FrameInterval = CurrentFrameTime - LastFrameTime;
	LastFrameTime = CurrentFrameTime;
}