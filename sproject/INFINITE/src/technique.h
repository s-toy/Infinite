#pragma once

class CTechnique
{
public:
	CTechnique();
	virtual ~CTechnique();

	virtual void initV() = 0;
};