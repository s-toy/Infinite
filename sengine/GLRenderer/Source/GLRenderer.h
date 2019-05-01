#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "BaseRenderer.h"
#include "GLRendererExport.h"

namespace sengine
{
	namespace renderEngine
	{
		class OPENGL_RENDERER_DLL_EXPORT CGLRenderer : public IBaseRenderer
		{
		public:
			CGLRenderer();
			virtual ~CGLRenderer();

			double getFrameInterval() const { return m_FrameInterval; }

		protected:
			virtual bool _initV() override;
			virtual bool _renderV() override;

			virtual void _handleEventV();

			virtual bool _isRenderLoopDoneV() override;

			void _registerKeyCallback(GLFWkeyfun vFun) { glfwSetKeyCallback(m_pWindow, vFun); }	//TODO: 还可以封装
			void _registerCursorPosCallback(GLFWcursorposfun vFun) { glfwSetCursorPosCallback(m_pWindow, vFun); }
			void _registerMouseButtonCallback(GLFWmousebuttonfun vFun) { glfwSetMouseButtonCallback(m_pWindow, vFun); }
			void _registerScrollCallback(GLFWscrollfun vFun) { glfwSetScrollCallback(m_pWindow, vFun); }

		private:
			bool __createGLFWWindow();
			void __updateFrameInterval();

		private:
			GLFWwindow *m_pWindow = nullptr;
			double m_FrameInterval = 0.0f;
		};
	}
}