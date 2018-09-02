#pragma once

class CTechnique
{
public:
	CTechnique();
	virtual ~CTechnique();

	virtual void initTechniqueV() = 0;
};