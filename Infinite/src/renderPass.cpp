#include "renderPass.h"
#include <boost/format.hpp>
#include "constants.h"
#include "textureUtil.h"
#include "gameShadingTechnique.h"

CRenderPass::CRenderPass() : m_RenderTexture(0), m_pShadingTechnique(nullptr)
{

}

CRenderPass::~CRenderPass()
{
	__destroy();
}

//*********************************************************************************
//FUNCTION:
void CRenderPass::init(const SPassConfig& vPassConfig)
{
	m_Config = vPassConfig;
	m_pShadingTechnique = CGameShadingTechnique::getInstance();

	__initTextures();
	__initShaders();
}

//*********************************************************************************
//FUNCTION:
void CRenderPass::__initTextures()
{
	for (auto& Channel : m_Config.channels)
	{
		if (Channel.first.empty()) continue;
		auto ChannelType = Channel.first;
		auto ChannelValue = Channel.second;

		if (ChannelType::BUFFER == ChannelType)
		{
			auto Index = atoi(ChannelValue.c_str());
			m_TextureSet.push_back(m_RenderTextureMap[Index]);
		}
		else if (ChannelType::TEXTURE == ChannelType)
		{
			auto TextureID = util::loadTexture(ChannelValue.c_str());
			_ASSERTE(TextureID > 0);
			m_TextureSet.push_back(TextureID);
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CRenderPass::__initShaders()
{
	auto pShadingPass = new CProgram;
	const SGameConfig& GameConfig = CGameConfig::getInstance()->getConfig();
	const SSceneConfig& SceneConfig = GameConfig.sceneConfigMap.at(m_CurrentSceneID);

	pShadingPass->addShader(Constant::DRAW_QUAD_SHADER_PATH, VERTEX_SHADER);
	std::vector<std::string> ShaderFilesPaths = { Constant::MAIN_IMAGE_SHADER_PATH };
	if (!SceneConfig.commonShaderPath.empty()) { ShaderFilesPaths.push_back(SceneConfig.commonShaderPath); }
	ShaderFilesPaths.push_back(m_Config.shaderPath);

	pShadingPass->addShader(ShaderFilesPaths, FRAGMENT_SHADER);
	auto PassName = boost::str(boost::format("pass%1%") % m_Config.passID);
	m_pShadingTechnique->addProgram(PassName, pShadingPass);
}

//*********************************************************************************
//FUNCTION:
void CRenderPass::__destroy()
{
	//TODO: release pass resource.
}
