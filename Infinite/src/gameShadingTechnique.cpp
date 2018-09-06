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

	auto pRenderKeyEvent2TexPass = new CProgram;
	pRenderKeyEvent2TexPass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	pRenderKeyEvent2TexPass->addShader("res/shaders/core/renderKeyEvent2Texture_fs.glsl", FRAGMENT_SHADER);
	this->addProgram("renderKeyEvent2TexPass", pRenderKeyEvent2TexPass);
}