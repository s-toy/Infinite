#include <iostream>
#include "RenderEngineInterface.h"
#include "constants.h"
#include "gameConfig.h"

int main()
{
	try
	{
		CGameConfig::getInstance()->init(constant::GAME_CONFIG_FILE_PATH);

		sengine::renderEngine::init();
		sengine::renderEngine::run();
	}
	catch (...)
	{
		std::cerr << "The program is terminated due to unexpected error." << std::endl;
	}

	system("pause");

	return 0;
}