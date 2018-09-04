#include "gameShadingTechnique.h"
#include <boost/format.hpp>
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

	auto SceneConfigSet = CGameConfig::getInstance()->getConfig().sceneConfigMap;
	for each (auto SceneConfig in SceneConfigSet)
	{
		auto pMainImagePass = new CShader;
		auto SceneID = SceneConfig.first;
		auto MainImageShaderPath = SceneConfig.second.mainImageShader;
		pMainImagePass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
		std::vector<std::string> MainImageShader = { "res/shaders/core/mainImage_fs.glsl", MainImageShaderPath };
		pMainImagePass->addShader(MainImageShader, FRAGMENT_SHADER);
		auto PassName = boost::str(boost::format("mainImagePass%1%") % SceneID);
		addShader(PassName, pMainImagePass);
	}

	auto pTextureCopyPass = new CShader;
	pTextureCopyPass->addShader("res/shaders/core/drawQuad_vs.glsl", VERTEX_SHADER);
	pTextureCopyPass->addShader("res/shaders/core/textureCopy_fs.glsl", FRAGMENT_SHADER);
	addShader("textureCopyPass", pTextureCopyPass);
}