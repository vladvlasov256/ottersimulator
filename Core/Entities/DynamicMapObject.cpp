//
//  ObstacleMapObject.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#include "global.h"
#include "DynamicMapObject.hpp"
#include "JSONHelper.hpp"
#include "DynamicObjectManager.hpp"

USING_NS_CC;
using namespace ottersimulator;

bool DynamicMapObject::init()
{
    if(!Entity::init())
    {
        return false;
    }
    
    typeName = "DynamicMapObject";
    sprite = nullptr;
    
    return true;
}

bool DynamicMapObject::initWithFile(const std::string &fileName)
{
    if(!DynamicMapObject::init())
    {
        return false;
    }
    
    this->setFileName(fileName);
    
    return true;
}

DynamicMapObject* DynamicMapObject::create(const std::string& fileName)
{
    DynamicMapObject *dynamicMapObject = new (std::nothrow) DynamicMapObject();
    if (dynamicMapObject && dynamicMapObject->initWithFile(fileName))
    {
        dynamicMapObject->autorelease();
        return dynamicMapObject;
    }
    CC_SAFE_DELETE(dynamicMapObject);
    return nullptr;
}

void DynamicMapObject::setFileName(const std::string& fileName)
{
    this->fileName = fileName;
    
    this->dynamicObject = DynamicObjectManager::getInstance()->getByFileName(this->fileName);
    if(this->dynamicObject)
    {
        setSprite(dynamicObject->getTextureFilePath());
    }
}

void DynamicMapObject::setSprite(const std::string& fileName)
{
    if(sprite)
    {
        removeChild(sprite);
    }
    
    if(fileName.empty())
    {
        this->sprite = nullptr;
        this->setSize(Size());
    }
    else
    {
        this->sprite = Sprite::create(fileName);
        Size windowSize = Director::getInstance()->getWinSize();
        this->sprite->setScale(UNITS_IN_SCREEN_HEIGHT / windowSize.height);
        addChild(this->sprite);
        
        Size contentSize = this->sprite->getContentSize();
        this->setSize(Size(contentSize.width / windowSize.height, contentSize.height / windowSize.height) * UNITS_IN_SCREEN_HEIGHT);
    }
}

bool DynamicMapObject::loadImpl(const rapidjson::Value& json)
{
    this->setFileName(json["filename"].GetString());
    return Entity::loadImpl(json);
}

bool DynamicMapObject::saveImpl(rapidjson::Document& document, rapidjson::Value& json)
{
    WriteString(document, json, "filename", this->fileName);
    return Entity::saveImpl(document, json);
}