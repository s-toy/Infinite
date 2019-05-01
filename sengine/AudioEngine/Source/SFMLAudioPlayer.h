#pragma once
#include <unordered_map>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include "IAudioEngineImp.h"
#include "AudioEngineCommon.h"

namespace sengine
{
	namespace audioEngine
	{
		class CFFTAudioStream;

		class CSFMLAudioPlayer : public IAudioEngineImp
		{
		public:
			CSFMLAudioPlayer();
			virtual ~CSFMLAudioPlayer();

		protected:
			virtual bool _initV() override;
			virtual AudioID _playAudio2DV(const std::string& vFilePath) override;
			virtual void _stopAllAudiosV() override;
			virtual void _destroyV() override;
			virtual int _getAudioDuration(AudioID vAudioID) const override;
			virtual void _dumpAudioSampleData(AudioID vAudioID, std::vector<float>& voSampleData) const override;

		private:
			AudioID m_AudioID = -1;
			std::unordered_map<AudioID, sf::Music*> m_ID2AudioMap;
			sf::SoundBuffer* m_pSoundBuffer = nullptr; //NOTE: m_pSoundBuffer与m_pFFTAudioStream只在m_GenerateAudioSamplesHint为true时被用到
			CFFTAudioStream* m_pFFTAudioStream = nullptr;
		};
	}
}