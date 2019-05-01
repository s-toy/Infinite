#pragma once
#include <common/BaseProduct.h>
#include "RenderEngineExport.h"

namespace sengine
{
	namespace renderEngine
	{
		class RENDER_ENGINE_DLL_EXPORT IBaseRenderer : public hiveOO::CBaseProduct
		{
			friend class CRenderEngine;

		public:
			virtual ~IBaseRenderer() {}

		protected:
			IBaseRenderer() {}

			virtual bool _initV() = 0;
			virtual bool _renderV() = 0;

			virtual bool _isRenderLoopDoneV() { return false; };
		};
	}
}