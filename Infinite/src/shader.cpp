#include "shader.h"
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

CShader::CShader()
{
	m_ProgramID = glCreateProgram();
}

CShader::~CShader()
{
	glDeleteProgram(m_ProgramID);
}

//*********************************************************************************
//FUNCTION:
void CShader::addShader(const std::string& vShaderName, EShaderType vShaderType)
{
	_ASSERT(!vShaderName.empty());
	std::string ShaderText = __readShaderFile(vShaderName);

	GLuint ShaderID = 0;
	switch (vShaderType)
	{
	case VERTEX_SHADER:
		ShaderID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT_SHADER:
		ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case GEOMETRY_SHADER:
		ShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case COMPUTE_SHADER:
		ShaderID = glCreateShader(GL_COMPUTE_SHADER);
		break;
	default:
		break;
	}

	const GLchar* pShaderText = ShaderText.c_str();
	glShaderSource(ShaderID, 1, &pShaderText, nullptr);
	__compileShader(ShaderID);

	glAttachShader(m_ProgramID, ShaderID);
	__linkProgram(m_ProgramID);

	_ASSERT(glGetError() == GL_NO_ERROR);
}

//*********************************************************************************
//FUNCTION:
void CShader::addShader(const std::vector<std::string>& vShaderNameSet, EShaderType vShaderType)
{
	_ASSERT(!vShaderNameSet.empty());
	std::string ShaderText = "";
	for (auto ShaderName : vShaderNameSet)
	{
		_ASSERT(!ShaderName.empty());
		ShaderText += __readShaderFile(ShaderName) + "\r\n";
	}

	GLuint ShaderID = 0;
	switch (vShaderType)
	{
	case VERTEX_SHADER:
		ShaderID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT_SHADER:
		ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case GEOMETRY_SHADER:
		ShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case COMPUTE_SHADER:
		ShaderID = glCreateShader(GL_COMPUTE_SHADER);
		break;
	default:
		break;
	}

	const GLchar* pShaderText = ShaderText.c_str();
	glShaderSource(ShaderID, 1, &pShaderText, nullptr);
	__compileShader(ShaderID);

	glAttachShader(m_ProgramID, ShaderID);
	__linkProgram(m_ProgramID);

	_ASSERT(glGetError() == GL_NO_ERROR);
}

//*********************************************************************************
//FUNCTION:
void CShader::setTextureUniformValue(const char *name, unsigned int n)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform1i(key, n) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setMat4UniformValue(const char *name, const glm::mat4 &mat)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	//	glUniformMatrix4fv(glGetUniformLocation(m_Program, name), 1, GL_FALSE, &mat[0][0]);
	key >= 0 ? glUniformMatrix4fv(key, 1, GL_FALSE, &mat[0][0]) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setMat3UniformValue(const char *name, const glm::mat3 &mat)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniformMatrix3fv(key, 1, GL_FALSE, &mat[0][0]) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setFloatUniformValue(const char *name, GLfloat v0)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform1f(key, v0) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform2f(key, v0, v1) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform3f(key, v0, v1, v2) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform4f(key, v0, v1, v2, v3) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setIntUniformValue(const char *name, GLint v0)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform1i(key, v0) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setIntUniformValue(const char *name, GLint v0, GLint v1)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform2i(key, v0, v1) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setIntUniformValue(const char *name, GLint v0, GLint v1, GLint v2)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform3i(key, v0, v1, v2) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
void CShader::setIntUniformValue(const char *name, GLint v0, GLint v1, GLint v2, GLint v3)
{
	GLint key = glGetUniformLocation(m_ProgramID, name);
	key >= 0 ? glUniform4i(key, v0, v1, v2, v3) : __printUniformWarningInfo(name);
}

//*********************************************************************************
//FUNCTION:
std::string CShader::__readShaderFile(const std::string& vFileName)
{
	_ASSERT(vFileName.size() != 0);
	GLchar* pShaderText = nullptr;
	std::filebuf* pFileBuffer = nullptr;
	unsigned int FileSize = 0;

	std::ifstream FileIn;
	FileIn.open(vFileName, std::ios::binary);

	if (FileIn.fail())
	{
		std::cout << "Fail to open " << std::endl;
		return nullptr;
	}

	pFileBuffer = FileIn.rdbuf();
	FileSize = (unsigned int)pFileBuffer->pubseekoff(0, std::ios::end, std::ios::in);
	pFileBuffer->pubseekpos(0, std::ios::in);

	pShaderText = new GLchar[FileSize + 1];
	pFileBuffer->sgetn(pShaderText, FileSize);
	pShaderText[FileSize] = '\0';
	FileIn.close();

	std::string ShaderText = pShaderText;
	delete[] pShaderText;

	return ShaderText;
}

//*********************************************************************************
//FUNCTION:
void CShader::__compileShader(GLuint& vShader)
{
	glCompileShader(vShader);

	GLint Compile;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &Compile);

	if (!Compile)
	{
		GLint LogLength;
		GLchar* pLog = nullptr;
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &LogLength);

		if (LogLength > 0)
		{
			pLog = new GLchar[LogLength];
			glGetShaderInfoLog(vShader, LogLength, &LogLength, pLog);
			fprintf(stderr, "Compile log = '%s' \n", pLog);
			delete[] pLog;
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CShader::__linkProgram(GLuint& vProgram)
{
	glLinkProgram(vProgram);

	GLint Link;
	glGetProgramiv(vProgram, GL_LINK_STATUS, &Link);

	if (!Link)
	{
		GLint LogLength;
		glGetProgramiv(vProgram, GL_INFO_LOG_LENGTH, &LogLength);

		if (LogLength > 0)
		{
			auto pLog = new GLchar[LogLength];
			glGetProgramInfoLog(vProgram, LogLength, &LogLength, pLog);
			fprintf(stderr, "Link log = '%s' \n", pLog);
			delete[] pLog;
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CShader::__printUniformWarningInfo(const std::string& vUniform)
{
	std::cout << "The Uniform '" << vUniform << "' does not exist or never be used: " << std::endl;
}
