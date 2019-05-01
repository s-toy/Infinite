#pragma once

#include "shadingTechnique.h"

class CGameShadingTechnique : public CShadingTechnique
{
public:
	virtual ~CGameShadingTechnique();

	static CGameShadingTechnique* getInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGameShadingTechnique();
		return m_pInstance;
	}

	virtual void initV() override;

private:
	CGameShadingTechnique();

private:
	static CGameShadingTechnique* m_pInstance;
};