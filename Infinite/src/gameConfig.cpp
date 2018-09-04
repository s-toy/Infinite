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
void CGameConfig::initConfig(const std::string& vConfigFileName)
{
	ptree GameConfig;
	read_json(vConfigFileName, GameConfig);

	m_Config.winName = GameConfig.get<std::string>("winName");

	auto WinSizeConfig = GameConfig.get_child("winSize");
	m_Config.winWidth = WinSizeConfig.get<int>("width");
	m_Config.winHeight = WinSizeConfig.get<int>("height");

	auto WinPosConfig = GameConfig.get_child("winPos");
	m_Config.winPosX = WinPosConfig.get<int>("x");
	m_Config.winPosY = WinPosConfig.get<int>("y");

	m_Config.isFullscreen = GameConfig.get<bool>("fullScreen");
	m_Config.entrySceneID = GameConfig.get<int>("entrySceneID");

	auto SceneConfigSet = GameConfig.get_child("scene");
	BOOST_FOREACH(ptree::value_type& Value, SceneConfigSet)
	{
		ptree Tree = Value.second;
		SSceneConfig TmpSceneConfig;
		auto Id = Tree.get<int>("id");
		TmpSceneConfig.mainImageShader = Tree.get<std::string>("mainImageShader");
		TmpSceneConfig.iChannel[0] = Tree.get<std::string>("iChannel0");
		TmpSceneConfig.iChannel[1] = Tree.get<std::string>("iChannel1");
		TmpSceneConfig.iChannel[2] = Tree.get<std::string>("iChannel2");
		TmpSceneConfig.iChannel[3] = Tree.get<std::string>("iChannel3");
		m_Config.sceneConfigMap.insert(std::make_pair(Id, TmpSceneConfig));
	}
}