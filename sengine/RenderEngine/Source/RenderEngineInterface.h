#pragma once
#include "RenderEngineExport.h"

namespace sengine
{
	namespace renderEngine
	{
		class CDisplayDevice;

		RENDER_ENGINE_DLL_EXPORT bool init();
		RENDER_ENGINE_DLL_EXPORT bool run();

		RENDER_ENGINE_DLL_EXPORT CDisplayDevice* fetchDisplayDevice();
	}
}