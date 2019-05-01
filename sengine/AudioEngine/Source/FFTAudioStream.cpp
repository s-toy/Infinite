#include "FFTAudioStream.h"
#include <SFML/Audio.hpp>
#include "Math/FFT.h"
#include "AudioEngineCommon.h"

using namespace sengine::audioEngine;

//*********************************************************************************
//FUNCTION:
void CFFTAudioStream::load(const sf::SoundBuffer* vSoundBuffer)
{
	m_SampleSet.assign(vSoundBuffer->getSamples(), vSoundBuffer->getSamples() + vSoundBuffer->getSampleCount());

	m_CurrentSampleSet.resize(SAMPLES_TO_STREAM);
	m_CurrentSamplePos = 0;

	initialize(vSoundBuffer->getChannelCount(), vSoundBuffer->getSampleRate());
}

//*********************************************************************************
//FUNCTION:
bool CFFTAudioStream::onGetData(sf::SoundStream::Chunk& vioData)
{
	__updateCurrentStreamSamples();

	CFFT::Forward(m_CurrentSampleSet.data(), SAMPLES_TO_STREAM);
	m_CurrentSpectrumSampleSet = m_CurrentSampleSet;

	CFFT::Inverse(m_CurrentSampleSet.data(), SAMPLES_TO_STREAM);
	m_CurrentWaveSampleSet = m_CurrentSampleSet;

	vioData.samples = &m_SampleSet[m_CurrentSamplePos];

	if (m_CurrentSamplePos + SAMPLES_TO_STREAM <= m_SampleSet.size()) {
		vioData.sampleCount = SAMPLES_TO_STREAM;
		m_CurrentSamplePos += SAMPLES_TO_STREAM;
		return true;
	}
	else {
		vioData.sampleCount = m_SampleSet.size() - m_CurrentSamplePos;
		m_CurrentSamplePos = m_SampleSet.size();
		return false;
	}
}

//*********************************************************************************
//FUNCTION:
void CFFTAudioStream::onSeek(sf::Time vTimeOffset)
{
	m_CurrentSamplePos = static_cast<std::size_t>(vTimeOffset.asSeconds() * getSampleRate() * getChannelCount());
}

//*********************************************************************************
//FUNCTION:
void CFFTAudioStream::__updateCurrentStreamSamples() {
	for (int i = 0; i < m_CurrentSampleSet.size(); i++)
		m_CurrentSampleSet[i] = m_SampleSet[m_CurrentSamplePos + i];
}