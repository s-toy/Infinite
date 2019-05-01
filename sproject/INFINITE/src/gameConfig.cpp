#include "gameConfig.h"
#include <iostream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <gl/glew.h>
#include <common/CommonInterface.h>
#include <common/HiveCommonMicro.h>
#include "common.h"
#include "constants.h"

using namespace boost::property_tree;

CGameConfig* CGameConfig::m_pInstance = nullptr;

CGameConfig::CGameConfig()
{
}

CGameConfig::~CGameConfig()
{
}

//*********************************************************************************
//FUNCTION:
void CGameConfig::init(const std::string& vConfigFileName)
{
	__parseGameConfig(vConfigFileName);
}

//*********************************************************************************
//FUNCTION:
void CGameConfig::__parseGameConfig(const std::string& vConfigFileName)
{
	_ASSERTE(!vConfigFileName.empty());

	ptree GameConfigTree;
	read_json(vConfigFileName, GameConfigTree);

	//TODO: 处理标签不存在的情况
	m_Config.entrySceneID = GameConfigTree.get<int>("entrySceneID");

	auto SceneConfigSet = GameConfigTree.get_child("scene");
	BOOST_FOREACH(const ptree::value_type& Value, SceneConfigSet)
	{
		ptree Tree = Value.second;
		SSceneConfig TmpSceneConfig;
		auto ID = Tree.get<int>("id");
		auto SceneConfigFile = Tree.get<std::string>("configFile");
		m_Config.sceneConfigMap.insert(std::make_pair(ID, __parseSceneConfig(SceneConfigFile)));
	}
}

//*********************************************************************************
//FUNCTION:
SSceneConfig CGameConfig::__parseSceneConfig(const std::string& vConfigFileName)
{
	_ASSERTE(!vConfigFileName.empty());

	ptree SceneConfigTree;
	try {
		boost::property_tree::read_json(vConfigFileName, SceneConfigTree);
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	_ASSERTE(!SceneConfigTree.empty());

	SSceneConfig SceneConfig;
	SceneConfig.commonShaderPath = SceneConfigTree.get_optional<std::string>("common").get_value_or("");

	const auto& PassConfigTree = SceneConfigTree.get_child("pass");
	BOOST_FOREACH(const ptree::value_type& Item, PassConfigTree)
	{
		SceneConfig.passConfigSet.push_back(__parsePassConfig(Item.second));
	}

	return SceneConfig;
}

//*********************************************************************************
//FUNCTION:
SPassConfig CGameConfig::__parsePassConfig(const ptree& vConfigItem)
{
	SPassConfig PassConfig;

	PassConfig.passID = vConfigItem.get<int>("id");
	PassConfig.type = __parsePassType(vConfigItem.get<std::string>("type"));
	PassConfig.shaderPath = vConfigItem.get<std::string>("shaderPath");

	const auto& ChannelConfigTree = vConfigItem.get_child_optional("channels");
	if (ChannelConfigTree) {
		BOOST_FOREACH(const ptree::value_type& Item, ChannelConfigTree.value())
		{
			PassConfig.channelConfigSet.push_back(__parseChannelConfig(Item.second));
		}
	}

	return PassConfig;
}

//*********************************************************************************
//FUNCTION:
SChannelConfig CGameConfig::__parseChannelConfig(const ptree& vConfigItem)
{
	SChannelConfig ChannelConfig;
	ChannelConfig.type = __parseChannelType(vConfigItem.get<std::string>("type"));
	ChannelConfig.value = vConfigItem.get_optional<std::string>("value").get_value_or("");
	ChannelConfig.filterMode = __parseFilterMode(vConfigItem.get_optional<std::string>("filterMode").get_value_or(""));
	ChannelConfig.wrapMode = __parseWrapMode(vConfigItem.get_optional<std::string>("wrapMode").get_value_or(""));
	ChannelConfig.vflip = vConfigItem.get_optional<bool>("vflip").get_value_or(Default::TEXTURE_IS_VFLIP);
	ChannelConfig.isMipmap = vConfigItem.get_optional<bool>("isMipmap").get_value_or(Default::TEXTURE_IS_MIPMAP);
	ChannelConfig.format = __parseTextureFormat(vConfigItem.get_optional<std::string>("format").get_value_or(""));

	return ChannelConfig;
}

//*********************************************************************************
//FUNCTION:
int CGameConfig::__parseFilterMode(const std::string& vModeStr)
{
	int FilterMode = Default::TEXTURE_FILTER_MODE;
	if (vModeStr.empty()) return FilterMode;

	if ("linear" == vModeStr) FilterMode = GL_LINEAR;
	else if ("nearest" == vModeStr) FilterMode = GL_NEAREST;
	else if ("mipmap" == vModeStr) FilterMode = GL_MIPMAP;

	return FilterMode;
}

//*********************************************************************************
//FUNCTION:
int CGameConfig::__parseWrapMode(const std::string& vModeStr)
{
	int WrapMode = Default::TEXTURE_WRAP_MODE;
	if (vModeStr.empty()) return WrapMode;

	if ("clamp" == vModeStr) WrapMode = GL_CLAMP;
	else if ("repeat" == vModeStr) WrapMode = GL_REPEAT;
	else if ("edge" == vModeStr) WrapMode = GL_CLAMP_TO_EDGE;
	else if ("border" == vModeStr) WrapMode = GL_CLAMP_TO_BORDER;

	return WrapMode;
}

//*********************************************************************************
//FUNCTION:
unsigned int CGameConfig::__parseTextureFormat(const std::string& vModeStr)
{
	if (vModeStr.empty()) return Default::TEXTURE_FORMAT;

	if ("red" == vModeStr || "r" == vModeStr)
		return GL_RED;
	else if ("rgb" == vModeStr)
		return GL_RGB;
	else if ("rgba" == vModeStr)
		return GL_RGBA;

	hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, _BOOST_STR1("Failed to parse texture format [%1].", vModeStr));

	return 0;
}

//*********************************************************************************
//FUNCTION:
EChannelType CGameConfig::__parseChannelType(const std::string& vTypeStr)
{
	_ASSERTE(!vTypeStr.empty());

	EChannelType Type;
	if ("buffer" == vTypeStr) Type = EChannelType::BUFFER;
	else if ("texture" == vTypeStr) Type = EChannelType::TEXTURE;
	else if ("cubeMap" == vTypeStr) Type = EChannelType::CUBE_MAP;
	else if ("keyboard" == vTypeStr) Type = EChannelType::KEYBOARD;
	else if ("sound" == vTypeStr) Type = EChannelType::SOUND;
	else if ("soundCloud" == vTypeStr) Type = EChannelType::SOUND_CLOUD;
	else if ("video" == vTypeStr) Type = EChannelType::VIDEO;
	else std::cerr << "undefined channel type." << std::endl;

	return Type;
}

//*********************************************************************************
//FUNCTION:
EPassType CGameConfig::__parsePassType(const std::string& vTypeStr)
{
	_ASSERTE(!vTypeStr.empty());

	EPassType Type;
	if ("buffer" == vTypeStr) Type = EPassType::BUFFER;
	else if ("image" == vTypeStr) Type = EPassType::IMAGE;
	else if ("cubeMap" == vTypeStr) Type = EPassType::CUBE_MAP;
	else if ("sound" == vTypeStr) Type = EPassType::SOUND;
	else std::cerr << "undefined pass type." << std::endl;

	return Type;
}
