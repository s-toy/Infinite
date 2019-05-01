#pragma once
#include <common/BaseProduct.h>
#include "AudioEngineExport.h"
#include "AudioEngineCommon.h"

namespace sengine
{
	namespace audioEngine
	{
		class AUDIO_ENGINE_DLL_EXPORT IAudioEngineImp : public hiveOO::CBaseProduct
		{
			friend class CAudioEngine;

		public:
			virtual ~IAudioEngineImp() = default;

		protected:
			IAudioEngineImp() = default;

			virtual bool _initV() = 0;
			virtual void _destroyV() = 0;

			virtual AudioID _playAudio2DV(const std::string& vFilePath) = 0;
			virtual void _stopAllAudiosV() = 0;

			virtual void _dumpAudioSampleData(AudioID vAudioID, std::vector<float>& voSampleData) const = 0;
			virtual int _getAudioDuration(AudioID vAudioID) const = 0;

			void _setGenerateAudioSampleHint() { m_GenerateAudioSamplesHint = true; }
			bool _isGenrateAudioSample() const { return m_GenerateAudioSamplesHint; }

		private:
			bool m_GenerateAudioSamplesHint = false;
		};
	}
}