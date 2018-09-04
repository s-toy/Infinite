#pragma once

#include <string>
#include <unordered_map>

struct SSceneConfig
{
	std::string mainImageShader;
	std::string iChannel[4];
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

	void initConfig(const std::string& vConfigFileName);
	SGameConfig getConfig() const { return m_Config; }

private:
	CGameConfig();

private:
	static CGameConfig* m_pInstance;
	SGameConfig m_Config;
};