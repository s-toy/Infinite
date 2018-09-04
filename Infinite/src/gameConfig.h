#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct SPassConfig
{
	int passID;
	std::string type;
	std::string shaderPath;
	std::vector<std::pair<std::string, std::string>> channels;

	SPassConfig() : passID(-1) {}
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

private:
	static CGameConfig* m_pInstance;
	SGameConfig m_Config;
};