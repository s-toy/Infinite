#pragma once

#include "shadingTechnique.h"

class CGameShadingTechnique : public CShadingTechnique
{
public:
	CGameShadingTechnique();
	virtual ~CGameShadingTechnique();

	virtual void initTechniqueV() override;
};