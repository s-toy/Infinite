#pragma once
#include <common/HiveConfig.h>

namespace sengine
{
	namespace renderEngine
	{
		class CRenderEngineConfig : public hiveConfig::CHiveConfig
		{
			friend class CRenderEngine;

		public:
			CRenderEngineConfig();
			virtual ~CRenderEngineConfig();

		protected:
			virtual void _loadDefaultV() override;
			virtual bool _onConfigFileMissedV() override;
			virtual hiveConfig::EConfigValidity _verifyConfigV() const override;

		private:
			void __defineAcceptableAttributeSet();
		};
	}
}