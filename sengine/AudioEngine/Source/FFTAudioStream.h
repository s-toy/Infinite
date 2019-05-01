#pragma once
#include <vector>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/SoundStream.hpp>
#include <SFML/System/Time.hpp>
#include "Math/Complex.h"

namespace sengine
{
	namespace audioEngine
	{
		class CFFTAudioStream : public sf::SoundStream
		{
		public:
			void load(const sf::SoundBuffer* vSoundBuffer);

			const std::vector<CComplex>& getCurrentWaveSampleSet() const { return m_CurrentWaveSampleSet; }
			const std::vector<CComplex>& getCurrentSpectrumSampleSet() const { return m_CurrentSpectrumSampleSet; }

		private:
			std::size_t m_CurrentSamplePos = 0;

			std::vector<sf::Int16> m_SampleSet;
			std::vector<CComplex> m_CurrentSampleSet;
			std::vector<CComplex> m_CurrentSpectrumSampleSet;
			std::vector<CComplex> m_CurrentWaveSampleSet;

			virtual bool onGetData(Chunk& vioData) override;
			virtual void onSeek(sf::Time vTimeOffset) override;

			void __updateCurrentStreamSamples();
		};
	}
}