#include "DXRenderer.h"
#include <windows.h>
#include <windowsx.h>
#include <common/HiveCommonMicro.h>
#include <common/CommonInterface.h>
#include <common/ProductFactory.h>
#include "RenderEngineInterface.h"
#include "DisplayDevice.h"

using namespace sengine::renderEngine;

hiveOO::CProductFactory<CDXRenderer> Creator("DXRenderer");

CDXRenderer::CDXRenderer()
{
}

CDXRenderer::~CDXRenderer()
{
}

//*********************************************************************************
//FUNCTION:
bool CDXRenderer::_initV()
{
	_HIVE_EARLY_RETURN(!__createDXWindow(), "Fail to initialize directX render engine due to failure of __createDXWindow().", false);
	hiveCommon::hiveOutputEvent("Succeed to init directX renderer.");

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CDXRenderer::_renderV()
{
	_handleEventV();

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CDXRenderer::_isRenderLoopDoneV()
{
	return m_IsRenderLoopDone;
}

//*********************************************************************************
//FUNCTION:
LRESULT CALLBACK __windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//*********************************************************************************
//FUNCTION:
bool CDXRenderer::__createDXWindow()
{
	auto pDisplayDevice = fetchDisplayDevice();
	_ASSERTE(pDisplayDevice);
	auto DisplayInfo = pDisplayDevice->getDisplayDeviceInfo();
	_ASSERTE(DisplayInfo.isValid());

	HWND hWnd;
	WNDCLASSEX wc;
	HMODULE hModule = GetModuleHandle(nullptr); //NOTE: 无法保证这种做法是正确的，是否要使用DLLMain传入的hModule

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = __windowProc;
	wc.hInstance = hModule;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1"; //TODO: 如何处理
	RegisterClassEx(&wc);

	hWnd = CreateWindowExA(NULL,
		"WindowClass1",						// name of the window class
		DisplayInfo.WinTitle.c_str(),		// title of the window
		WS_OVERLAPPEDWINDOW,				// window style
		DisplayInfo.WinPosX,				// x-position of the window
		DisplayInfo.WinPosY,				// y-position of the window
		DisplayInfo.WinWidth,				// width of the window
		DisplayInfo.WinHeight,				// height of the window
		NULL,								// parent window
		NULL,								// menus
		hModule,							// application handle
		NULL);								// used with multiple windows, NULL

	ShowWindow(hWnd, 1); //TODO: 第二个参数的作用

	return true;
}

//*********************************************************************************
//FUNCTION:
void CDXRenderer::_handleEventV()
{
	MSG msg;
	if (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else
	{
		m_IsRenderLoopDone = true;
	}
}