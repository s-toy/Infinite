#include "DisplayDevice.h"
#include "Platform.h"
#include <common/CommonInterface.h>
#include <common/HiveCommonMicro.h>

using namespace sengine::renderEngine;

CDisplayDevice::CDisplayDevice()
{

}

CDisplayDevice::~CDisplayDevice()
{

}

//*********************************************************************************
//FUNCTION:
bool CDisplayDevice::init()
{
	_HIVE_EARLY_RETURN(!__queryScreenSize(m_DisplayInfo), "Fail to initialize engine due to failure of __queryScreenSize().", false);

	return true;
}

//*********************************************************************************
//FUNCTION:
void CDisplayDevice::createWindow(const std::string& vTitle, unsigned int vWidth, unsigned int vHeight, unsigned int vPosX, unsigned int vPosY)
{
	_ASSERTE(m_DisplayInfo.isValid());

	if (vWidth > m_DisplayInfo.ScreenWidth)   vWidth = m_DisplayInfo.ScreenWidth;
	if (vHeight > m_DisplayInfo.ScreenHeight) vHeight = m_DisplayInfo.ScreenHeight;

	m_DisplayInfo.WinTitle = vTitle;
	m_DisplayInfo.WinWidth = vWidth;
	m_DisplayInfo.WinHeight = vHeight;
	m_DisplayInfo.WinPosX = vPosX;
	m_DisplayInfo.WinPosY = vPosY;
}

//*********************************************************************************
//FUNCTION:
void CDisplayDevice::createWindow(const std::string& vTitle, unsigned int vWidth, unsigned int vHeight)
{
	_ASSERTE(m_DisplayInfo.isValid());

	if (vWidth > m_DisplayInfo.ScreenWidth)   vWidth = m_DisplayInfo.ScreenWidth;
	if (vHeight > m_DisplayInfo.ScreenHeight) vHeight = m_DisplayInfo.ScreenHeight;

	unsigned int PosX = (m_DisplayInfo.ScreenWidth - vWidth) / 2;
	unsigned int PosY = (m_DisplayInfo.ScreenHeight - vHeight) / 2;

	createWindow(vTitle, vWidth, vHeight, PosX, PosY);
}

//*********************************************************************************
//FUNCTION:
void CDisplayDevice::createFullScreenWindow(const std::string& vTitle)
{
	createWindow(vTitle, m_DisplayInfo.ScreenWidth, m_DisplayInfo.ScreenHeight, 0, 0);
	m_DisplayInfo.IsFullScreenWindow = true;
}

//*********************************************************************************
//FUNCTION:
bool CDisplayDevice::__queryScreenSize(SDisplayInfo& voDisplayInfo) const
{
#ifdef _WINDOWS
	voDisplayInfo.ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	voDisplayInfo.ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
#else
	hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Fail to query screen size due to the platform is not supported.");
	return false;
#endif // _WINDOWS

	return true;
}