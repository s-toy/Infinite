#include "RenderEngineInterface.h"
#include <common/CommonInterface.h>
#include <common/HiveCommonMicro.h>
#include "RenderEngine.h"

using namespace sengine::renderEngine;

//*********************************************************************************
//FUNCTION:
bool sengine::renderEngine::init()
{
	try
	{
		return CRenderEngine::getInstance()->init();
	}
	catch (...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Fail to initialize render engine due to unexpected error.");
		return false;
	}
}

//*********************************************************************************
//FUNCTION:
bool sengine::renderEngine::run()
{
	bool IsNormalExit = CRenderEngine::getInstance()->run();
	CRenderEngine::getInstance()->destroy();

	return IsNormalExit;
}

//*********************************************************************************
//FUNCTION:
CDisplayDevice* sengine::renderEngine::fetchDisplayDevice()
{
	return CRenderEngine::getInstance()->fetchDisplayDevice();
}