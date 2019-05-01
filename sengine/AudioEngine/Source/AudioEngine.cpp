#include "AudioEngine.h"
#include <boost/format.hpp>
#include <common/CommonInterface.h>
#include <common/HiveCommonMicro.h>
#include <common/OOInterface.h>
#include "AudioEngineConfig.h"
#include "AudioEngineCommon.h"
#include "IAudioEngineImp.h"

using namespace sengine::audioEngine;

CAudioEngine::CAudioEngine()
{

}

CAudioEngine::~CAudioEngine()
{
	destroy();
	_SAFE_DELETE(m_pAudioEngineImp);
}

//*********************************************************************************
//FUNCTION:
bool CAudioEngine::init()
{
	if (m_IsInitialized) return true;

	std::string EngineSig = CAudioEngineConfig::getInstance()->getAttribute<std::string>(CONFIG_KEYWORD::AUDIO_ENGINE_SIGNATURE);
	_ASSERTE(!EngineSig.empty());

	m_pAudioEngineImp = dynamic_cast<IAudioEngineImp*>(hiveOO::hiveGetOrCreateProduct(EngineSig));
	_HIVE_EARLY_RETURN(!m_pAudioEngineImp, _BOOST_STR1("Fail to initialize audio engine due to bad engine object signature [%1%].", EngineSig), false);
	_HIVE_EARLY_RETURN(!m_pAudioEngineImp->_initV(), "Fail to initialize audio engine due to failure of initialzing concrete engine object.", false);

	hiveCommon::hiveOutputEvent("Succeed to init audio engine.");

	m_IsInitialized = true;
	return true;
}

//*********************************************************************************
//FUNCTION:
AudioID CAudioEngine::playAudio2D(const std::string& vFilePath)
{
	_ASSERTE(!vFilePath.empty());
	if (!m_IsInitialized) init();
	if (!m_IsInitialized) return 0;

	return m_pAudioEngineImp->_playAudio2DV(vFilePath);
}

//*********************************************************************************
//FUNCTION:
void CAudioEngine::stopAllAudios() const
{
	if (!m_IsInitialized) return;

	m_pAudioEngineImp->_stopAllAudiosV();
}

//*********************************************************************************
//FUNCTION:
void CAudioEngine::destroy()
{
	m_IsInitialized = false;
	if (m_pAudioEngineImp)
		m_pAudioEngineImp->_destroyV();
}

//*********************************************************************************
//FUNCTION:
void CAudioEngine::dumpAudioSampleData(AudioID vAudioID, std::vector<float>& voSampleData) const
{
	m_pAudioEngineImp->_dumpAudioSampleData(vAudioID, voSampleData);
}

//*********************************************************************************
//FUNCTION:
int CAudioEngine::getAudioDuration(AudioID vAudioID) const
{
	return m_pAudioEngineImp->_getAudioDuration(vAudioID);
}

//*********************************************************************************
//FUNCTION:
void CAudioEngine::setGenerateAudioSampleHint()
{
	if (!m_IsInitialized) init();
	m_pAudioEngineImp->_setGenerateAudioSampleHint();
}