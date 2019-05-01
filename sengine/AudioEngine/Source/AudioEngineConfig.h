#pragma once
#include <common/HiveConfig.h>
#include <common/Singleton.h>

namespace sengine
{
	namespace audioEngine
	{
		class CAudioEngineConfig : public virtual hiveConfig::CHiveConfig, public virtual hiveOO::CSingleton<CAudioEngineConfig>
		{
		public:
			virtual ~CAudioEngineConfig();

		private:
			CAudioEngineConfig();

			void __defineAcceptableAttributeSet();

			friend class hiveOO::CSingleton<CAudioEngineConfig>;
		};
	}
}