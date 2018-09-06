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

	auto pTextureCopyPass = new CProgram;
	pTextureCopyPass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	pTextureCopyPass->addShader("res/shaders/core/textureCopy_fs.glsl", FRAGMENT_SHADER);
	this->addProgram("textureCopyPass", pTextureCopyPass);
}