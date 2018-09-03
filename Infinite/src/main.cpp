#include "gameRenderer.h"
#include "constants.h"
#include "gameConfig.h"

int main()
{
	CGameRenderer* pRenderer = new CGameRenderer();
	_ASSERT(pRenderer);

	CGameConfig::getInstance()->initConfig(Constant::GAME_CONFIG_FILE_NAME);
	auto Config = CGameConfig::getInstance()->getConfig();

	pRenderer->initV(Config.winName, Config.winWidth, Config.winHeight, Config.winPosY, Config.winPosY, Config.isFullscreen);
	pRenderer->runV();

	delete pRenderer;

	return 0;
}