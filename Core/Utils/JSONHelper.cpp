//
//  JSONHelper.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 31.05.16.
//
//

#include "rapidjson/prettywriter.h" //!!!include folders
#include "rapidjson/stringbuffer.h" //!!!include folders
#include "JSONHelper.hpp"

USING_NS_CC;
using namespace ottersimulator;

void ottersimulator::LoadJSONFromFile(const std::string& filePath, rapidjson::Document& document, char*& buffer)
{
    FILE* fp = fopen(filePath.c_str(), "r");
    fseek(fp, 0, SEEK_END);
    size_t filesize = (size_t)ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = (char*)malloc(filesize + 1);
    size_t readLength = fread(buffer, 1, filesize, fp);
    buffer[readLength] = '\0';
    fclose(fp);
    
    document.ParseInsitu<rapidjson::kParseDefaultFlags>(buffer);
}

void ottersimulator::SaveJSONToFile(const std::string& filePath, rapidjson::Document& document)
{
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    
    document.Accept(writer);
    
    FILE* fp = fopen(filePath.c_str(), "wb");
    fprintf(fp, "%s", buffer.GetString());
    fclose(fp);
}

Vec2 ottersimulator::ParseVec2(const rapidjson::Value& vector)
{
    return Vec2(vector[(rapidjson::SizeType)0].GetDouble(), vector[(rapidjson::SizeType)1].GetDouble());
}

void ottersimulator::WriteVec2(rapidjson::Document& document, rapidjson::Value& json, const char* fieldName, const Vec2& vector)
{
    rapidjson::Value vectorValue(rapidjson::kArrayType);
    vectorValue.PushBack(rapidjson::Value().SetDouble(vector.x), document.GetAllocator());
    vectorValue.PushBack(rapidjson::Value().SetDouble(vector.y), document.GetAllocator());
    
    json.AddMember(fieldName, vectorValue, document.GetAllocator());
}

void ottersimulator::WriteString(rapidjson::Document& document, rapidjson::Value& json, const char* fieldName, const std::string& value)
{
    rapidjson::Value stringValue(rapidjson::kStringType);
    stringValue.SetString(value.c_str(), document.GetAllocator());
    json.AddMember(fieldName, stringValue, document.GetAllocator());
}