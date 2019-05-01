#include "RenderEngineConfig.h"
#include "RenderEngineCommon.h"
#include "common/ProductFactory.h"

using namespace sengine::renderEngine;

CRenderEngineConfig::CRenderEngineConfig()
{
	setType(CONFIG_FILE_TYPE::RENDER_ENGINE_CONFIG);
	__defineAcceptableAttributeSet();
}

CRenderEngineConfig::~CRenderEngineConfig()
{

}

//*********************************************************************************
//FUNCTION:
void CRenderEngineConfig::_loadDefaultV()
{
	setAttribute(CONFIG_KEYWORD::WIN_POSITION, CONFIG_DEFAULT::DEFAULT_WIN_POSITION);
	setAttribute(CONFIG_KEYWORD::WIN_SIZE, CONFIG_DEFAULT::DEFAULT_WIN_SIZE);
	setAttribute(CONFIG_KEYWORD::WIN_TITLE, CONFIG_DEFAULT::DEFAULT_WIN_TITLE);
	setAttribute(CONFIG_KEYWORD::WIN_IS_FULLSCREEN, false);
	setAttribute(CONFIG_KEYWORD::WIN_IS_RESIZABLE, false);

	setAttribute(CONFIG_KEYWORD::RENDER_ENGINE_SIGNATURE, CONFIG_DEFAULT::DEFAULT_RENDER_ENGINE_SIGNATURE);
}

//*********************************************************************************
//FUNCTION:
bool CRenderEngineConfig::_onConfigFileMissedV()
{
	_loadDefaultV();
	return true;
}

//*********************************************************************************
//FUNCTION:
hiveConfig::EConfigValidity CRenderEngineConfig::_verifyConfigV() const
{
	return hiveConfig::EConfigValidity::OK;
}

//*********************************************************************************
//FUNCTION:
void CRenderEngineConfig::__defineAcceptableAttributeSet()
{
	defineAttribute(CONFIG_KEYWORD::WIN_POSITION, hiveConfig::ATTRIBUTE_VEC2I);
	defineAttribute(CONFIG_KEYWORD::WIN_SIZE, hiveConfig::ATTRIBUTE_VEC2I);
	defineAttribute(CONFIG_KEYWORD::WIN_TITLE, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(CONFIG_KEYWORD::WIN_IS_FULLSCREEN, hiveConfig::ATTRIBUTE_BOOL);
	defineAttribute(CONFIG_KEYWORD::WIN_IS_RESIZABLE, hiveConfig::ATTRIBUTE_BOOL);

	defineAttribute(CONFIG_KEYWORD::RENDER_ENGINE_SIGNATURE, hiveConfig::ATTRIBUTE_STRING);
}