#include "SFMLAudioPlayer.h"
#include <common/HiveCommonMicro.h>
#include <common/CommonInterface.h>
#include <common/ProductFactory.h>
#include "FFTAudioStream.h"

using namespace sengine::audioEngine;

hiveOO::CProductFactory<CSFMLAudioPlayer> Creator("SFMLAudioPlayer");

CSFMLAudioPlayer::CSFMLAudioPlayer()
{

}

CSFMLAudioPlayer::~CSFMLAudioPlayer()
{
	_destroyV();
}

//*********************************************************************************
//FUNCTION:
bool CSFMLAudioPlayer::_initV()
{
	return true;
}

//*********************************************************************************
//FUNCTION:
AudioID CSFMLAudioPlayer::_playAudio2DV(const std::string& vFilePath)
{
	_ASSERTE(!vFilePath.empty());

	if (!_isGenrateAudioSample())
	{
		auto pMusic = new sf::Music;
		if (!pMusic->openFromFile(vFilePath)) { _SAFE_DELETE(pMusic); return -1; }

		pMusic->play();
		m_ID2AudioMap[++m_AudioID] = pMusic;
	}
	else //NOTE: m_GenerateAudioSamplesHint为true是只能同时加载一个音频
	{
		if (m_pSoundBuffer) { _SAFE_DELETE(m_pSoundBuffer); }
		if (m_pFFTAudioStream) { _SAFE_DELETE(m_pFFTAudioStream); }
		m_pSoundBuffer = new sf::SoundBuffer;
		if (!m_pSoundBuffer->loadFromFile(vFilePath)) { return -1; }

		m_pFFTAudioStream = new CFFTAudioStream;
		m_pFFTAudioStream->load(m_pSoundBuffer);
		m_pFFTAudioStream->play();
		return 0;
	}

	return m_AudioID;
}

//*********************************************************************************
//FUNCTION:
void CSFMLAudioPlayer::_stopAllAudiosV()
{
	if (m_pFFTAudioStream) m_pFFTAudioStream->stop();
	for (auto e : m_ID2AudioMap) e.second->stop();
}

//*********************************************************************************
//FUNCTION:
void CSFMLAudioPlayer::_destroyV()
{
	_SAFE_DELETE(m_pFFTAudioStream);
	_SAFE_DELETE(m_pSoundBuffer);
	for (auto e : m_ID2AudioMap) _SAFE_DELETE(e.second);

	m_ID2AudioMap.clear();
	m_AudioID = -1;
}

//*********************************************************************************
//FUNCTION:
int CSFMLAudioPlayer::_getAudioDuration(AudioID vAudioID) const
{
	if (_isGenrateAudioSample())
	{
		_ASSERTE(m_pSoundBuffer);
		return m_pSoundBuffer->getDuration().asMilliseconds();
	}
	else {
		const auto Iter = m_ID2AudioMap.find(vAudioID);
		if (Iter == m_ID2AudioMap.end()) return -1;

		auto pMusic = Iter->second;
		_ASSERTE(pMusic);
		return pMusic->getDuration().asMilliseconds();
	}
}

//*********************************************************************************
//FUNCTION:
void CSFMLAudioPlayer::_dumpAudioSampleData(AudioID vAudioID, std::vector<float>& voSampleData) const
{
	if (!_isGenrateAudioSample()) return;

	_ASSERTE(m_pFFTAudioStream);
	const auto& SampleSpectrumVector = m_pFFTAudioStream->getCurrentSpectrumSampleSet();
	for (auto i : SampleSpectrumVector) voSampleData.emplace_back(i.re());
}