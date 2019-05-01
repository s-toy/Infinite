#include "AudioEngineConfig.h"
#include <common/ProductFactory.h>
#include "AudioEngineCommon.h"

using namespace sengine::audioEngine;

CAudioEngineConfig::CAudioEngineConfig()
{
	__defineAcceptableAttributeSet();
}

CAudioEngineConfig::~CAudioEngineConfig()
{

}

//*********************************************************************************
//FUNCTION:
void CAudioEngineConfig::__defineAcceptableAttributeSet()
{
	defineAttribute(CONFIG_KEYWORD::AUDIO_ENGINE_SIGNATURE, hiveConfig::ATTRIBUTE_STRING);
	setAttribute(CONFIG_KEYWORD::AUDIO_ENGINE_SIGNATURE, CONFIG_DEFAULT::DEFAULT_AUDIO_ENGINE_SIGNATURE);
}