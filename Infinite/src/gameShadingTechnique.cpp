#include "gameShadingTechnique.h"
#include <boost/format.hpp>
#include "gameConfig.h"

CGameShadingTechnique* CGameShadingTechnique::m_pInstance = nullptr;

CGameShadingTechnique::CGameShadingTechnique()
{
}

CGameShadingTechnique::~CGameShadingTechnique()
{
}

//*********************************************************************************
//FUNCTION:
void CGameShadingTechnique::initV()
{
	CShadingTechnique::initV();
}