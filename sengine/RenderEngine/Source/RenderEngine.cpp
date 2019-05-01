#include "RenderEngine.h"
#include <boost/format.hpp>
#include <common/OOInterface.h>
#include <common/CommonInterface.h>
#include <common/HiveCommonMicro.h>
#include <common/ConfigInterface.h>
#include "RenderEngineCommon.h"
#include "BaseRenderer.h"
#include "Platform.h"
#include "DisplayDevice.h"

using namespace sengine::renderEngine;

CRenderEngine::CRenderEngine() : m_IsInitialized(false), m_IsRunning(false), m_IsRenderLoopDone(false), m_pRenderer(nullptr), m_pDisplayDevice(nullptr)
{

}

CRenderEngine::~CRenderEngine()
{
	_SAFE_DELETE(m_pRenderer);
	_SAFE_DELETE(m_pDisplayDevice);
}

//*********************************************************************************
//FUNCTION:
bool CRenderEngine::init()
{
	if (m_IsInitialized) return true;

	bool IsParseConfigSuccessed = hiveConfig::hiveParseConfig(CONFIG_FILE_NAME::RENDER_ENGINE_CONFIG_FILE_NAME, hiveConfig::EConfigType::XML, &m_RenderEngineConfig);
	_HIVE_EARLY_RETURN(!IsParseConfigSuccessed, "Fail to initialize engine due to failure of parsing render engine configuration file.", false);

	std::string EngineSig = m_RenderEngineConfig.getAttribute<std::string>(CONFIG_KEYWORD::RENDER_ENGINE_SIGNATURE);
	_ASSERTE(!EngineSig.empty());
	//HACK: The name of the library must be same as the engine signature, this is not flexible enough.
	_HIVE_EARLY_RETURN(!__loadRendererLibrary(EngineSig), _BOOST_STR1("Fail to load render engine library [%1%].", EngineSig), false);

	m_pDisplayDevice = new CDisplayDevice();
	_HIVE_EARLY_RETURN(!m_pDisplayDevice->init(), "Fail to initialize engine due to failure of initialzing display device.", false);
	__createWindow();

	m_pRenderer = dynamic_cast<IBaseRenderer*>(hiveOO::hiveGetOrCreateProduct(EngineSig));
	_HIVE_EARLY_RETURN(!m_pRenderer, _BOOST_STR1("Fail to initialize engine due to bad engine object signature [%1%].", EngineSig), false);
	_HIVE_EARLY_RETURN(!m_pRenderer->_initV(), "Fail to initialize engine due to failure of initialzing concrete engine object.", false);

	hiveCommon::hiveOutputEvent("Succeed to init render engine.");

	m_IsInitialized = true;
	return true;
}

//*********************************************************************************
//FUNCTION:
bool CRenderEngine::run()
{
	_HIVE_EARLY_RETURN(!m_IsInitialized, "Fail to start engine because it has not been initialized.", false);
	_ASSERTE(m_pRenderer);

	bool IsNormalExit = true;
	try
	{
		m_IsRunning = true;
		while (!m_IsRenderLoopDone)
		{
			if (!__render())
			{
				hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Render loop interrupted due to render failure.");
				IsNormalExit = false;
				break;
			}

			m_IsRenderLoopDone = m_pRenderer->_isRenderLoopDoneV();
		}
	}
	catch (...)
	{
		IsNormalExit = false;
	}
	m_IsRunning = false;
	return IsNormalExit;
}

//*********************************************************************************
//FUNCTION:
bool CRenderEngine::__render()
{
	m_pRenderer->_renderV();

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CRenderEngine::__loadRendererLibrary(const std::string& vRendererSig)
{
#ifdef _WINDOWS
	HINSTANCE Handle;
#ifdef _DEBUG
	Handle = LoadLibrary(_BOOST_STR1("%1%D.dll", vRendererSig).c_str());
#else
	Handle = LoadLibrary(_BOOST_STR1("%1%.dll", vRendererSig).c_str());
#endif // _DEBUG
	if (!Handle) { FreeLibrary(Handle); return false; }
#else 
	return false;
#endif // _WINDOWS

	return true;
}

//*********************************************************************************
//FUNCTION:
void CRenderEngine::__createWindow()
{
	_ASSERTE(m_pDisplayDevice);
	std::string WinTitle = m_RenderEngineConfig.getAttribute<std::string>(CONFIG_KEYWORD::WIN_TITLE);
	boost::tuple<int, int> WinSize = m_RenderEngineConfig.getAttribute<boost::tuple<int, int>>(CONFIG_KEYWORD::WIN_SIZE);
	boost::tuple<int, int> WinPos = m_RenderEngineConfig.getAttribute<boost::tuple<int, int>>(CONFIG_KEYWORD::WIN_POSITION);
	bool IsFullScreen = m_RenderEngineConfig.getAttribute<bool>(CONFIG_KEYWORD::WIN_IS_FULLSCREEN);

	if (!IsFullScreen)
		m_pDisplayDevice->createWindow(WinTitle, WinSize.get<0>(), WinSize.get<1>(), WinPos.get<0>(), WinPos.get<1>());
	else
		m_pDisplayDevice->createFullScreenWindow(WinTitle);

	bool IsWinResizalbe = m_RenderEngineConfig.getAttribute<bool>(CONFIG_KEYWORD::WIN_IS_RESIZABLE);
	if (IsWinResizalbe) m_pDisplayDevice->setWindowResizableHint();
}