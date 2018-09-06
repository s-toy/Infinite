#pragma once

#include <map>
#include "program.h"
#include "technique.h"

class CShadingTechnique : public CTechnique
{
public:
	CShadingTechnique();
	virtual ~CShadingTechnique();

	virtual void initV() override {};

	void addProgram(const std::string& vProgramSig, CProgram* vProgram);
	void removeProgram(const std::string& vProgramSig);
	void removeAllProgram();

	void updateStandShaderUniform(const std::string& vUniformName, boost::any vValue);
	void updateTextureShaderUniform(const std::string& vUniformName, int vValue);

	void enableProgram(const std::string& vProgramSig);
	void disableProgram();
	unsigned int getProgramID(const std::string& vProgramSig);

protected:
	std::map<std::string, CProgram*> m_ProgramSet;
	CProgram* m_CurrentProgram;
};