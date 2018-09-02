#pragma once

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"  
#include "json/stringbuffer.h"  

USING_NS_CC;

class CJsonReader
{
public:
	CJsonReader(const std::string& vFilename);
	~CJsonReader();

	std::string readString(const std::string& vAttributName);
	int readInt(const std::string& vAttributName);
	double readDouble(const std::string& vAttributName);
	Size readSize(const std::string& vAttributName);
	bool readBool(const std::string& vAttributName);

	cocos2d::Color4B parseColor4B(const rapidjson::Value& vValue);
	cocos2d::Vec2 parseVec2(const rapidjson::Value& vValue);

	const rapidjson::Document& getDocument() { return m_Doc; }

private:
	rapidjson::Document m_Doc;
};

class CJsonWriter
{
public:
	CJsonWriter(const std::string& vFilename);
	~CJsonWriter();

	void writeBool(const std::string& vAttributName, bool vValue);
	void writeSize(const std::string& vAttributName, Size vValue);

	void saveFile();

private:
	std::string m_FileName;
	rapidjson::Document m_Doc;
};