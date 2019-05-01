#pragma once
#include <string>

namespace sengine
{
	namespace renderEngine
	{
		struct SDisplayInfo
		{
			unsigned int ScreenWidth = 0, ScreenHeight = 0;
			unsigned int WinWidth = 0, WinHeight = 0;
			unsigned int WinPosX = 0, WinPosY = 0;
			std::string	 WinTitle;
			bool IsFullScreenWindow = false;
			bool IsWindowResizable = false;

			bool isValid() { return ((ScreenWidth > 0) && (ScreenHeight > 0)); }
		};

		class CDisplayDevice
		{
		public:
			CDisplayDevice();
			virtual ~CDisplayDevice();

			bool init();

			void createFullScreenWindow(const std::string& vTitle);
			void createWindow(const std::string& vTitle, unsigned int vWidth, unsigned int vHeight);
			void createWindow(const std::string& vTitle, unsigned int vWidth, unsigned int vHeight, unsigned int vPosX, unsigned int vPosY);

			void setWindowResizableHint() { m_DisplayInfo.IsWindowResizable = true; }

			const SDisplayInfo& getDisplayDeviceInfo() const { return m_DisplayInfo; }

		private:
			SDisplayInfo m_DisplayInfo;

			bool __queryScreenSize(SDisplayInfo& voDisplayInfo) const;
		};
	}
}