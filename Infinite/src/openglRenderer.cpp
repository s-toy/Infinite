#include "openglRenderer.h"

COpenGLRenderer::COpenGLRenderer() :m_pWindow(nullptr), m_DeltaTime(0.0f), m_LastFrame(0.0f)
{

}

COpenGLRenderer::~COpenGLRenderer()
{

}

//*********************************************************************************
//FUNCTION:
void COpenGLRenderer::initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_pWindow = glfwCreateWindow(vWindowWidth, vWindowHeight, vWindowTitle.c_str(), nullptr, nullptr);
	_ASSERTE(m_pWindow);
	glfwMakeContextCurrent(m_pWindow);
	glewExperimental = GL_TRUE;

	glewInit();
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//*********************************************************************************
//FUNCTION:
void COpenGLRenderer::runV()
{
	while (!glfwWindowShouldClose(m_pWindow))
	{
		GLfloat CurrentFrame = glfwGetTime();
		m_DeltaTime = CurrentFrame - m_LastFrame;
		m_LastFrame = CurrentFrame;

		glfwPollEvents();

		_handleEventsV();
		_updateV();

		glfwSwapBuffers(m_pWindow);
	}
	glfwTerminate();
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
