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
	pRenderBarragePass->addShader("res/shaders/drawQuad_vs.glsl", VERTEX_SHADER);
	pRenderBarragePass->addShader("res/shaders/renderBarrage_fs.glsl", FRAGMENT_SHADER);
	addShader("RenderBarragePass", pRenderBarragePass);

	auto pTextureCopyPass = new CShader;
	pTextureCopyPass->addShader("res/shaders/drawQuad_vs.glsl", VERTEX_SHADER);
	pTextureCopyPass->addShader("res/shaders/textureCopy_fs.glsl", FRAGMENT_SHADER);
	addShader("TextureCopyPass", pTextureCopyPass);

	auto pRenderPlayerPass = new CShader;
	pRenderPlayerPass->addShader("res/shaders/drawQuad_vs.glsl", VERTEX_SHADER);
	pRenderPlayerPass->addShader("res/shaders/renderPlayer_fs.glsl", FRAGMENT_SHADER);
	addShader("RenderPlayerPass", pRenderPlayerPass);
}