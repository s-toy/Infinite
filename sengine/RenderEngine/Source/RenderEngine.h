#pragma once
#include <common/Singleton.h>
#include "RenderEngineConfig.h"

namespace sengine
{
	namespace renderEngine
	{
		class IBaseRenderer;
		class CDisplayDevice;
		class CRenderEngineConfig;

		class CRenderEngine : public hiveOO::CSingleton<CRenderEngine>
		{
			friend class hiveOO::CSingleton<CRenderEngine>;

		public:
			virtual ~CRenderEngine();

			bool init();
			bool run();

			IBaseRenderer*	fetchRenderer() const { return m_pRenderer; }
			CDisplayDevice*		fetchDisplayDevice() const { return m_pDisplayDevice; }

		private:
			bool m_IsInitialized;
			bool m_IsRunning;
			bool m_IsRenderLoopDone;

			IBaseRenderer	*m_pRenderer;
			CDisplayDevice		*m_pDisplayDevice;

			CRenderEngineConfig m_RenderEngineConfig;

			CRenderEngine();

			bool __render();
			bool __loadRendererLibrary(const std::string& vRendererSig);
			void __createWindow();
		};
	}
}