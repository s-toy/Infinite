#include "JsonUtil.h"

using namespace std;

CJsonReader::CJsonReader(const std::string& vFilename)
{
	ssize_t size = 0;
	unsigned char *pBytes = cocos2d::CCFileUtils::getInstance()->getFileData(vFilename, "r", &size);
	_ASSERTE(pBytes != nullptr && strcmp((char*)pBytes, "") != 0);
	std::string load_str((const char*)pBytes, size);
	m_Doc.Parse<0>(load_str.c_str());
	_ASSERTE(!m_Doc.HasParseError());
	_ASSERTE(!m_Doc.IsArray());
}

CJsonReader::~CJsonReader()
{

}

//**********************************************************************************************************
//FUNCTION:
string CJsonReader::readString(const string& vAttributName)
{
	const rapidjson::Value &p = m_Doc;
	_ASSERTE(p.HasMember(vAttributName.c_str()));
	return p[vAttributName.c_str()].GetString();
}

//**********************************************************************************************************
//FUNCTION:
int CJsonReader::readInt(const string& vAttributName)
{
	const rapidjson::Value &p = m_Doc;
	_ASSERTE(p.HasMember(vAttributName.c_str()));
	return p[vAttributName.c_str()].GetInt();
}

//**********************************************************************************************************
//FUNCTION:
double CJsonReader::readDouble(const string& vAttributName)
{
	const rapidjson::Value &p = m_Doc;
	_ASSERTE(p.HasMember(vAttributName.c_str()));
	return p[vAttributName.c_str()].GetDouble();
}

//**********************************************************************************************************
//FUNCTION:
Size CJsonReader::readSize(const string& vAttributName)
{
	const rapidjson::Value &p = m_Doc;
	_ASSERTE(p.HasMember(vAttributName.c_str()));
	double width = p[vAttributName.c_str()]["width"].GetDouble();
	double height = p[vAttributName.c_str()]["height"].GetDouble();
	return Size(width, height);
}

//**********************************************************************************************************
//FUNCTION:
bool CJsonReader::readBool(const string& vAttributName)
{
	const rapidjson::Value &p = m_Doc;
	_ASSERTE(p.HasMember(vAttributName.c_str()));
	return p[vAttributName.c_str()].GetBool();
}

//**********************************************************************************************************
//FUNCTION:
cocos2d::Color4B CJsonReader::parseColor4B(const rapidjson::Value& vValue)
{
	Color4B Color;
	Color.r = vValue["r"].GetFloat();
	Color.g = vValue["g"].GetFloat();
	Color.b = vValue["b"].GetFloat();
	Color.a = vValue["a"].GetFloat();

	return Color;
}

//**********************************************************************************************************
//FUNCTION:
cocos2d::Vec2 CJsonReader::parseVec2(const rapidjson::Value& vValue)
{
	return Vec2(vValue["x"].GetFloat(), vValue["y"].GetFloat());
}

CJsonWriter::CJsonWriter(const std::string& vFilename)
{
	m_FileName = vFilename;

	ssize_t size = 0;
	unsigned char *pBytes = cocos2d::CCFileUtils::getInstance()->getFileData(vFilename, "r", &size);
	_ASSERTE(pBytes != nullptr && strcmp((char*)pBytes, "") != 0);
	std::string load_str((const char*)pBytes, size);
	m_Doc.Parse<0>(load_str.c_str());
	_ASSERTE(!m_Doc.HasParseError());
	_ASSERTE(!m_Doc.IsArray());
}

CJsonWriter::~CJsonWriter()
{

}

//**********************************************************************************************************
//FUNCTION:
void CJsonWriter::writeBool(const string& vAttributName, bool vValue)
{
	rapidjson::Value &p = m_Doc;
	_ASSERTE(p.HasMember(vAttributName.c_str()));
	p[vAttributName.c_str()].SetBool(vValue);
}

//**********************************************************************************************************
//FUNCTION:
void CJsonWriter::writeSize(const string& vAttributName, Size vValue)
{
	rapidjson::Value &p = m_Doc;
	_ASSERTE(p.HasMember(vAttributName.c_str()));
	p[vAttributName.c_str()]["width"].SetDouble(vValue.width);
	p[vAttributName.c_str()]["height"].SetDouble(vValue.height);
}

//**********************************************************************************************************
//FUNCTION:
void CJsonWriter::saveFile()
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	m_Doc.Accept(writer);

	_ASSERTE(cocos2d::CCFileUtils::getInstance()->writeStringToFile(buffer.GetString(), m_FileName));
}