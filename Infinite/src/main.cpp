#include "gameRenderer.h"
#include "constants.h"

int main()
{
	CGameRenderer* pRenderer = new CGameRenderer();
	_ASSERT(pRenderer);

	pRenderer->initV(Constant::WIN_NAME, Constant::WIN_WIDTH, Constant::WIN_HEIGHT);
	pRenderer->runV();

	delete pRenderer;

	return 0;
}