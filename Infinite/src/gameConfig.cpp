#include "gameConfig.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "common.h"

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
	m_Config.winName = GameConfigTree.get<std::string>("winName");

	auto WinSizeConfig = GameConfigTree.get_child("winSize");
	m_Config.winWidth = WinSizeConfig.get<int>("width");
	m_Config.winHeight = WinSizeConfig.get<int>("height");

	auto WinPosConfig = GameConfigTree.get_child("winPos");
	m_Config.winPosX = WinPosConfig.get<int>("x");
	m_Config.winPosY = WinPosConfig.get<int>("y");

	m_Config.isFullscreen = GameConfigTree.get<bool>("fullScreen");
	m_Config.entrySceneID = GameConfigTree.get<int>("entrySceneID");

	auto SceneConfigSet = GameConfigTree.get_child("scene");
	BOOST_FOREACH(ptree::value_type& Value, SceneConfigSet)
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
	read_json(vConfigFileName, SceneConfigTree);

	SSceneConfig SceneConfig;
	SceneConfig.commonShaderPath = SceneConfigTree.get_optional<std::string>("common").get_value_or("");

	ptree PassConfigTree = SceneConfigTree.get_child("pass");
	BOOST_FOREACH(ptree::value_type& E, PassConfigTree)
	{
		ptree Tree = E.second;
		SPassConfig PassConfig;

		PassConfig.passID = Tree.get<int>("id");
		PassConfig.type = Tree.get<std::string>("type");
		PassConfig.shaderPath = Tree.get<std::string>("shaderPath");

		auto ChannelItem = Tree.get_child_optional("channels");
		if (ChannelItem) {
			BOOST_FOREACH(ptree::value_type& E, ChannelItem.value())
			{
				ptree Tree = E.second;
				auto Type = Tree.get<std::string>("type");
				auto Value = Tree.get<std::string>("value");
				PassConfig.channels.push_back(std::make_pair(Type, Value));
			}
		}

		SceneConfig.passConfigSet.push_back(PassConfig);
	}

	return SceneConfig;
}