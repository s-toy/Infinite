#pragma once
#include <irrKlang.h>
#include <unordered_map>
#include "IAudioEngineImp.h"
#include "AudioEngineCommon.h"

namespace sengine
{
	namespace audioEngine
	{
		class CIrrklangAudioPlayer : public IAudioEngineImp
		{
		public:
			CIrrklangAudioPlayer();
			virtual ~CIrrklangAudioPlayer();

		protected:
			virtual bool _initV() override;
			virtual AudioID _playAudio2DV(const std::string& vFilePath) override;
			virtual void _stopAllAudiosV() override;
			virtual void _destroyV() override;
			virtual int _getAudioDuration(AudioID vAudioID) const override;
			virtual void _dumpAudioSampleData(AudioID vAudioID, std::vector<float>& voSampleData) const override;

		private:
			irrklang::ISoundEngine* m_SoundEngine = nullptr;
			std::unordered_map<AudioID, irrklang::ISound*> m_ID2SoundMap;
			AudioID m_AudioID = -1; //TODO: bad design
		};
	}
}