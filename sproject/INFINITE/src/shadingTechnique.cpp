#include "shadingTechnique.h"
#include <boost/any.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "common.h"

CShadingTechnique::CShadingTechnique() : m_CurrentProgram(nullptr)
{

}

CShadingTechnique::~CShadingTechnique()
{
	for (auto iter : m_ProgramSet)
		delete iter.second;

	m_ProgramSet.clear();
}

//*********************************************************************************
//FUNCTION:
void CShadingTechnique::addProgram(const std::string& vProgramSig, CProgram* vProgram)
{
	_ASSERT(!vProgramSig.empty());
	_ASSERT(!(vProgram == nullptr));

	auto ShaderSetIter = m_ProgramSet.find(vProgramSig);
	if (ShaderSetIter == m_ProgramSet.end())
	{
		m_ProgramSet.insert(std::make_pair(vProgramSig, vProgram));
	}
	else
	{
		std::cout << "\nError: Program Signature repetition!" << std::endl;
		_ASSERT(false);
	}
}

//*********************************************************************************
//FUNCTION:
void CShadingTechnique::removeProgram(const std::string& vProgramSig)
{
	auto Iter = m_ProgramSet.find(vProgramSig);
	if (Iter != m_ProgramSet.end())
	{
		//TODO: release program resources.
		auto pProgram = Iter->second;
		SAFE_DELETE(pProgram);
		m_ProgramSet.erase(Iter);
	}
}

//*********************************************************************************
//FUNCTION:
void CShadingTechnique::removeAllProgram()
{
	for each (auto E in m_ProgramSet)
	{
		auto pProgram = E.second;
		SAFE_DELETE(pProgram);
	}
	m_ProgramSet.clear();
}

//*********************************************************************************
//FUNCTION:
void CShadingTechnique::updateStandShaderUniform(const std::string& vUniformName, boost::any vValue)
{
	_ASSERT(!vUniformName.empty());
	if (m_CurrentProgram)
	{
		_ASSERT(m_CurrentProgram);
		const std::type_info& typeInfo = vValue.type();
		if (typeInfo == typeid(glm::mat4))
		{
			glm::mat4 mat = boost::any_cast<glm::mat4>(vValue);
			m_CurrentProgram->setMat4UniformValue(vUniformName.c_str(), mat);
		}
		else if (typeInfo == typeid(glm::mat3))
		{
			glm::mat3 mat = boost::any_cast<glm::mat3>(vValue);
			m_CurrentProgram->setMat3UniformValue(vUniformName.c_str(), mat);
		}
		else if (typeInfo == typeid(glm::vec4))
		{
			glm::vec4 v = boost::any_cast<glm::vec4>(vValue);
			m_CurrentProgram->setFloatUniformValue(vUniformName.c_str(), v[0], v[1], v[2], v[3]);
		}
		else if (typeInfo == typeid(glm::vec3))
		{
			glm::vec3 v = boost::any_cast<glm::vec3>(vValue);
			m_CurrentProgram->setFloatUniformValue(vUniformName.c_str(), v[0], v[1], v[2]);
		}
		else if (typeInfo == typeid(glm::vec2))
		{
			glm::vec2 v = boost::any_cast<glm::vec2>(vValue);
			m_CurrentProgram->setFloatUniformValue(vUniformName.c_str(), v[0], v[1]);
		}
		else if (typeInfo == typeid(float))
		{
			float v = boost::any_cast<float>(vValue);
			m_CurrentProgram->setFloatUniformValue(vUniformName.c_str(), v);
		}
		else if (typeInfo == typeid(double))
		{
			double v = boost::any_cast<double>(vValue);
			m_CurrentProgram->setFloatUniformValue(vUniformName.c_str(), (GLfloat)v);
		}
		else if (typeInfo == typeid(bool))
		{
			bool v = boost::any_cast<bool>(vValue);
			m_CurrentProgram->setIntUniformValue(vUniformName.c_str(), v);
		}
		else if (typeInfo == typeid(int))
		{
			int v = boost::any_cast<int>(vValue);
			m_CurrentProgram->setIntUniformValue(vUniformName.c_str(), v);
		}
		else if (typeInfo == typeid(unsigned int))
		{
			unsigned int v = boost::any_cast<unsigned int>(vValue);
			m_CurrentProgram->setIntUniformValue(vUniformName.c_str(), v);
		}
		else
		{
			std::cout << vUniformName << " type is not defined" << std::endl;
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CShadingTechnique::updateTextureShaderUniform(const std::string& vUniformName, int vValue)
{
	_ASSERT(!vUniformName.empty());
	if (m_CurrentProgram)
	{
		m_CurrentProgram->setTextureUniformValue(vUniformName.c_str(), vValue);
	}
}

//*********************************************************************************
//FUNCTION:
void CShadingTechnique::enableProgram(const std::string& vProgramSig)
{
	if (m_ProgramSet.find(vProgramSig) == m_ProgramSet.end()) return;

	m_CurrentProgram = m_ProgramSet[vProgramSig];
	m_CurrentProgram->enableShader();
}

//*********************************************************************************
//FUNCTION:
void CShadingTechnique::disableProgram()
{
	m_CurrentProgram = nullptr;
	glUseProgram(0);
}

//*********************************************************************************
//FUNCTION:
unsigned int CShadingTechnique::getProgramID(const std::string& vProgramSig)
{
	if (m_ProgramSet.find(vProgramSig) == m_ProgramSet.end()) return 0;
	m_CurrentProgram = m_ProgramSet[vProgramSig];
	return m_CurrentProgram->getProgramID();
}