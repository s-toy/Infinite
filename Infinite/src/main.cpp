#include "gameRenderer.h"
#include "constants.h"
#include "gameConfig.h"

int main()
{
	auto pGameConfig = CGameConfig::getInstance();
	pGameConfig->init(Constant::GAME_CONFIG_FILE_PATH);
	auto Config = pGameConfig->getConfig();

	auto pGameRenderer = CGameRenderer::getInstance();
	pGameRenderer->initV(Config.winName, Config.winWidth, Config.winHeight, Config.winPosY, Config.winPosY, Config.isFullscreen);

	return pGameRenderer->runV();
}