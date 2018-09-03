#include "gameShadingTechnique.h"
#include "gameConfig.h"

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

	auto pMainImagePass = new CShader;
	pMainImagePass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	auto CustomShaderPath = CGameConfig::getInstance()->getConfig().mainImageShader;
	std::vector<std::string> MainImageShader = { "res/shaders/core/mainImage_fs.glsl", CustomShaderPath };
	pMainImagePass->addShader(MainImageShader, FRAGMENT_SHADER);
	addShader("mainImagePass", pMainImagePass);

	auto pTextureCopyPass = new CShader;
	pTextureCopyPass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	pTextureCopyPass->addShader("res/shaders/core/textureCopy_fs.glsl", FRAGMENT_SHADER);
	addShader("textureCopyPass", pTextureCopyPass);

	auto pRenderPlayerPass = new CShader;
	pRenderPlayerPass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	pRenderPlayerPass->addShader("res/shaders/core/renderPlayer_fs.glsl", FRAGMENT_SHADER);
	addShader("renderPlayerPass", pRenderPlayerPass);
}