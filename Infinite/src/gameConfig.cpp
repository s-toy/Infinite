#include "gameConfig.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "common.h"

using namespace boost::property_tree;

CGameConfig* CGameConfig::m_pInstance = nullptr;

CGameConfig::CGameConfig()
{
}

CGameConfig::~CGameConfig()
{
}

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

	m_Config.mainImageShader = GameConfig.get<std::string>("mainImageShader");
	m_Config.iChannel[0]= GameConfig.get<std::string>("iChannel0");
	m_Config.iChannel[1] = GameConfig.get<std::string>("iChannel1");
	m_Config.iChannel[2] = GameConfig.get<std::string>("iChannel2");
	m_Config.iChannel[3] = GameConfig.get<std::string>("iChannel3");

}