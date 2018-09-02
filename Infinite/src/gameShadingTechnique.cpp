#include "gameShadingTechnique.h"

CGameShadingTechnique::CGameShadingTechnique()
{
}

CGameShadingTechnique::~CGameShadingTechnique()
{
}

//*********************************************************************************
//FUNCTION:
void CGameShadingTechnique::initTechniqueV()
{
	CShadingTechnique::initTechniqueV();

	auto pRenderBarragePass = new CShader;
	pRenderBarragePass->addShader("res/shaders/RenderBarrage_VS.glsl", VERTEX_SHADER);
	pRenderBarragePass->addShader("res/shaders/RenderBarrage_FS.glsl", FRAGMENT_SHADER);
	addShader("RenderBarragePass", pRenderBarragePass);

	auto pTextureCopyPass = new CShader;
	pTextureCopyPass->addShader("res/shaders/DrawScreenQuad_VS.glsl", VERTEX_SHADER);
	pTextureCopyPass->addShader("res/shaders/TextureCopy_FS.glsl", FRAGMENT_SHADER);
	addShader("TextureCopyPass", pTextureCopyPass);
}