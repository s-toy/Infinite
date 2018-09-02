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
	pRenderBarragePass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	std::vector<std::string> RenderBarrageShaders = { "res/shaders/core/mainImage_fs.glsl", "res/shaders/custom/sky_fs.glsl" };
	pRenderBarragePass->addShader(RenderBarrageShaders, FRAGMENT_SHADER);
	addShader("RenderBarragePass", pRenderBarragePass);

	auto pTextureCopyPass = new CShader;
	pTextureCopyPass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	pTextureCopyPass->addShader("res/shaders/core/textureCopy_fs.glsl", FRAGMENT_SHADER);
	addShader("TextureCopyPass", pTextureCopyPass);

	auto pRenderPlayerPass = new CShader;
	pRenderPlayerPass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	pRenderPlayerPass->addShader("res/shaders/core/renderPlayer_fs.glsl", FRAGMENT_SHADER);
	addShader("RenderPlayerPass", pRenderPlayerPass);
}