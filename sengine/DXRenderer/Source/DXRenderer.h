#pragma once
#include "BaseRenderer.h"
#include "DXRendererExport.h"

namespace sengine
{
	namespace renderEngine
	{
		class DIRECTX_RENDERER_DLL_EXPORT CDXRenderer : public IBaseRenderer
		{
		public:
			CDXRenderer();
			virtual ~CDXRenderer();

		protected:
			virtual bool _initV() override;
			virtual bool _renderV() override;
			virtual bool _isRenderLoopDoneV() override;

			virtual void _handleEventV();

		private:
			bool m_IsRenderLoopDone = false;

			bool __createDXWindow();
		};
	}
}