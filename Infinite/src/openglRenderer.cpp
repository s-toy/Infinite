#include "openglRenderer.h"

COpenGLRenderer::COpenGLRenderer() :m_pWindow(nullptr), m_DeltaTime(0.0f), m_LastFrame(0.0f)
{

}

COpenGLRenderer::~COpenGLRenderer()
{

}

//*********************************************************************************
//FUNCTION:
void COpenGLRenderer::initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, int vWinPosX, int vWinPosY, bool vIsFullscreen)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWmonitor *pMonitor = vIsFullscreen ? glfwGetPrimaryMonitor() : nullptr;
	m_pWindow = glfwCreateWindow(vWindowWidth, vWindowHeight, vWindowTitle.c_str(), pMonitor, nullptr);
	_ASSERTE(m_pWindow);
	glfwSetWindowPos(m_pWindow, vWinPosX, vWinPosY);
	glfwMakeContextCurrent(m_pWindow);
	glewExperimental = GL_TRUE;

	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//*********************************************************************************
//FUNCTION:
int COpenGLRenderer::runV()
{
	while (!glfwWindowShouldClose(m_pWindow))
	{
		float CurrentFrame = (float)glfwGetTime();
		m_DeltaTime = CurrentFrame - m_LastFrame;
		m_LastFrame = CurrentFrame;

		glfwPollEvents();

		_handleEventsV();
		_updateV();

		glfwSwapBuffers(m_pWindow);
	}
	glfwTerminate();

	return 0;
}

//*********************************************************************************
//FUNCTION:
void COpenGLRenderer::_updateV()
{

}

//*********************************************************************************
//FUNCTION:
void COpenGLRenderer::_handleEventsV()
{

}
