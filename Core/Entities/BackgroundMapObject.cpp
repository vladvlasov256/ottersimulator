//
//  BackgroundMapObject.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#include "global.h"
#include "BackgroundMapObject.hpp"
#include "JSONHelper.hpp"

using namespace ottersimulator;
using namespace cocos2d;

bool BackgroundMapObject::initWithFile(const std::string &fileName)
{
    if(!BackgroundMapObject::init())
    {
        return false;
    }
    
    setSprite(fileName);
    
    return true;
}

bool BackgroundMapObject::init()
{
    if(!Entity::init())
    {
        return false;
    }
    
    typeName = "BackgroundMapObject";
    sprite = nullptr;
    
    return true;
}

BackgroundMapObject* BackgroundMapObject::create(const std::string& fileName)
{
    BackgroundMapObject *backgroundMapObject = new (std::nothrow) BackgroundMapObject();
    if (backgroundMapObject && backgroundMapObject->initWithFile(fileName))
    {
        backgroundMapObject->autorelease();
        return backgroundMapObject;
    }
    CC_SAFE_DELETE(backgroundMapObject);
    return nullptr;
}
//
//bool BackgroundMapObject::isTouched(const Vec2& pos) const
//{
//}

void BackgroundMapObject::setSprite(const std::string& fileName)
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
        this->sprite->setScale(UNITS_IN_SCREEN_HEIGHT / windowSize.height * Director::getInstance()->getContentScaleFactor() * .5f);
        addChild(this->sprite);
        
        Size contentSize = this->sprite->getContentSize();
        this->setSize(Size(contentSize.width / windowSize.height, contentSize.height / windowSize.height) * UNITS_IN_SCREEN_HEIGHT);
    }
}

bool BackgroundMapObject::loadImpl(const rapidjson::Value& json)
{
    setSprite(json["filename"].GetString());
    
    return Entity::loadImpl(json);
}

bool BackgroundMapObject::saveImpl(rapidjson::Document& document, rapidjson::Value& json)
{
    WriteString(document, json, "filename", this->sprite ? this->sprite->getResourceName() : "");
    
    return Entity::saveImpl(document, json);
}