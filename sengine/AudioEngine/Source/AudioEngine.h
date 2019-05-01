#pragma once
#include <string>
#include <vector>
#include <common/Singleton.h>
#include "AudioEngineExport.h"
#include "AudioEngineCommon.h"

namespace sengine
{
	namespace audioEngine
	{
		class IAudioEngineImp;

		class CAudioEngine : public hiveOO::CSingleton<CAudioEngine>
		{
		public:
			CAudioEngine();
			virtual ~CAudioEngine();

			bool init();
			void destroy();

			AudioID playAudio2D(const std::string& vFilePath);
			void stopAllAudios() const;

			void setGenerateAudioSampleHint();
			void dumpAudioSampleData(AudioID vAudioID, std::vector<float>& voSampleData) const;
			int getAudioDuration(AudioID vAudioID) const;

		private:
			bool m_IsInitialized = false;
			IAudioEngineImp* m_pAudioEngineImp = nullptr;

			friend class hiveOO::CSingleton<CAudioEngine>;
		};
	}
}