#pragma once

#include <map>
#include "shader.h"
#include "technique.h"

class CShadingTechnique : public CTechnique
{
public:
	CShadingTechnique();
	virtual ~CShadingTechnique();

	virtual void initTechniqueV() override {};

	void addShader(const std::string& vShaderSig, CShader* vShader);

	void updateStandShaderUniform(const std::string& vUniformName, boost::any vValue);
	void updateTextureShaderUniform(const std::string& vUniformName, int vValue);

	void enableShader(const std::string& vShaderSig);
	void disableShader();
	unsigned int getProgramID(const std::string& vShaderSig);

protected:
	std::map<std::string, CShader*> m_ShaderSet;

	CShader* m_CurrentShader;
};