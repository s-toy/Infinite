#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>
#include "common.h"

struct SChannelConfig
{
	EChannelType type;
	std::string value;
	int filterMode;
	int wrapMode;
	bool vflip;
};

struct SPassConfig
{
	int passID;
	EPassType type;
	std::string shaderPath;
	std::vector<SChannelConfig> channelConfigSet;

	SPassConfig() : passID(-1) {}

	bool isValid() const { return true; } //TODO: 验证配置是否有效
};

struct SSceneConfig
{
	std::string commonShaderPath;
	std::vector<SPassConfig> passConfigSet;
};

struct SGameConfig
{
	std::string winName;
	int winWidth;
	int winHeight;
	int winPosX;
	int winPosY;
	bool isFullscreen;
	int entrySceneID;

	std::unordered_map<unsigned int, SSceneConfig> sceneConfigMap;

	SGameConfig() : winWidth(0), winHeight(0), winPosX(0), winPosY(0), isFullscreen(false), entrySceneID(0) {}
};

class CGameConfig
{
public:
	virtual ~CGameConfig();

	static CGameConfig* getInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGameConfig();
		return m_pInstance;
	}

	void init(const std::string& vConfigFileName);
	SGameConfig getConfig() const { return m_Config; }

private:
	CGameConfig();

	void __parseGameConfig(const std::string& vConfigFileName);
	SSceneConfig __parseSceneConfig(const std::string& vConfigFileName);
	SPassConfig __parsePassConfig(const boost::property_tree::ptree& vConfigItem);
	SChannelConfig __parseChannelConfig(const boost::property_tree::ptree& vConfigItem);

	int __parseFilterMode(const std::string& vModeStr);
	int __parseWrapMode(const std::string& vModeStr);
	EChannelType __parseChannelType(const std::string& vTypeStr);
	EPassType __parsePassType(const std::string& vTypeStr);

private:
	static CGameConfig* m_pInstance;
	SGameConfig m_Config;
};