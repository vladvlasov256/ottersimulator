//
//  JSONHelper.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 31.05.16.
//
//

#ifndef JSONHelper_hpp
#define JSONHelper_hpp

#include "cocos2d.h"
#include "rapidjson/document.h"

namespace ottersimulator
{
    void LoadJSONFromFile(const std::string& fileName, rapidjson::Document& document, char*& buffer);
    void SaveJSONToFile(const std::string& fileName, rapidjson::Document& document);
    
    cocos2d::Vec2 ParseVec2(const rapidjson::Value& vector);
    void WriteVec2(rapidjson::Document& document, rapidjson::Value& json, const char* fieldName, const cocos2d::Vec2& vector);
    void WriteString(rapidjson::Document& document, rapidjson::Value& json, const char* fieldName, const std::string& value);
}

#endif /* JSONHelper_hpp */
