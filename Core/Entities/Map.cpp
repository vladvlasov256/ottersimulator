//
//  Level.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#include "global.h"
#include "Map.hpp"
#include "Landscape.hpp"
#include "ResourceManager.hpp"
#include "BackgroundMapObject.hpp"
#include "DynamicMapObject.hpp"
#include "JSONHelper.hpp"

USING_NS_CC;
using namespace ottersimulator;

#define LANDSCAPE_Z 0
#define INITIAL_BACKGROUND_OBJECT_Z -10
#define INITIAL_DYNAMIC_OBJECT_Z 1
#define BACKGROUND_Z -1000

bool ottersimulator::Map::init()
{
    if(!Entity::init())
    {
        return false;
    }
    
    typeName = "Map";
    
    setBackgroundSpriteTextureFileName("");
    
    return true;
}

bool ottersimulator::Map::loadImpl(const rapidjson::Value& json)
{
    setBackgroundSpriteTextureFileName(json["background"].GetString());
   
    return Entity::loadImpl(json);
}

bool ottersimulator::Map::saveImpl(rapidjson::Document& document, rapidjson::Value& json)
{
    Vec2 position = getPosition();
    this->setPosition(Vec2());
    
    float scale = getScale();
    this->setScale(1.f);
    
    WriteString(document, json, "background", backgroundSprite ? backgroundSprite->getResourceName() : "");
    
    bool result = Entity::saveImpl(document, json);
    
    this->setScale(scale);
    this->setPosition(position);
    
    return result;
}

void ottersimulator::Map::setBackgroundSpriteTextureFileName(const std::string& fileName)
{
    if(backgroundSprite)
    {
        removeChild(backgroundSprite);
    }
    
    if(fileName.empty())
    {
        backgroundSprite = nullptr;
    }
    else
    {
        backgroundSprite = Sprite::create(fileName);
        Entity::addChild(backgroundSprite, BACKGROUND_Z);
        updateBackground();
    }
}

//Landscape* ottersimulator::Map::getLandscape() const
//{
//    auto childrenEnd = getChildren().end();
//    auto it = std::find_if(getChildren().begin(), childrenEnd, [](const Node* node)
//    {
//        auto entity = dynamic_cast<const Entity*>(node);
//        return entity && (entity->getTypeName() == "Landscape");
//    });
//    
//    return (it == childrenEnd) ? NULL : static_cast<Landscape*>(*it);
//}

void ottersimulator::Map::setVisibleRect(const cocos2d::Rect& rect)
{
    this->visibleRect = rect;
    
    Size windowSize = Director::getInstance()->getWinSize();
    
    float scale = windowSize.height / rect.size.height;// / UNITS_IN_SCREEN_HEIGHT;
    setScale(scale);
    
//    Vec2 center = rect.origin + rect.size * .5f;
//    Vec2 offset = (Vec2(windowSize.x / windowSize.y, 1.f) * .5f * UNITS_IN_SCREEN_HEIGHT - center) * scale;
    Vec2 offset = -rect.origin * scale;
    setPosition(offset);
    
    if(landscape)
    {
        landscape->setVisibleRect(rect);
    }
    
    updateBackground();
}

BackgroundMapObject* ottersimulator::Map::addBackgroundMapObject(const std::string& fileName)
{
    auto backgroundMapObject = BackgroundMapObject::create(fileName);
    this->addBackgroundMapObject(backgroundMapObject);
    return backgroundMapObject;
}

void ottersimulator::Map::addBackgroundMapObject(BackgroundMapObject* backgroundMapObject)
{
    Entity::addChild(backgroundMapObject, INITIAL_BACKGROUND_OBJECT_Z);
    this->backgroundObjects.push_back(backgroundMapObject);
}

DynamicMapObject* ottersimulator::Map::addDynamicMapObject(const std::string& fileName)
{
    auto DynamicMapObject = DynamicMapObject::create(fileName);
    this->addDynamicMapObject(DynamicMapObject);
    return DynamicMapObject;
}

void ottersimulator::Map::addDynamicMapObject(DynamicMapObject* dynamicObject)
{
    Entity::addChild(dynamicObject, INITIAL_DYNAMIC_OBJECT_Z);
    this->dynamicObjects.push_back(dynamicObject);
}

Landscape* ottersimulator::Map::createFlatLandscape()
{
    landscape = Landscape::createFlatBlock();
    addLandscape(landscape);
    return landscape;
}

void ottersimulator::Map::addLandscape(Landscape* landscape)
{
    this->landscape = landscape;
    Entity::addChild(landscape, LANDSCAPE_Z);
}

void ottersimulator::Map::updateBackground()
{
    if(backgroundSprite)
    {
        auto size = Director::getInstance()->getWinSize();
        backgroundSprite->setScale(size.height / backgroundSprite->getContentSize().height / getScale());
        backgroundSprite->setPosition((Point(size.width / 2, size.height / 2) - getPosition()) / getScale());
    }
}

Entity* ottersimulator::Map::getTouchedObject(const cocos2d::Vec2& pos) const
{//!!!dirty, two methods more
    auto dynamicObjectIt = std::find_if(dynamicObjects.begin(), dynamicObjects.end(), [&pos](DynamicMapObject* object)
                                 {
                                     return object->isTouched(pos);
                                 });
    
    if(dynamicObjectIt != dynamicObjects.end())
    {
        return *dynamicObjectIt;
    }
    
    auto backgroundObjectIt = std::find_if(backgroundObjects.begin(), backgroundObjects.end(), [&pos](BackgroundMapObject* object)
                 {
                     return object->isTouched(pos);
                 });
    
    if(backgroundObjectIt != backgroundObjects.end())
    {
        return *backgroundObjectIt;
    }
    
    return nullptr;
}

void ottersimulator::Map::addChild(cocos2d::Node * child)
{
    Landscape* landscape = dynamic_cast<Landscape*>(child);
    if(landscape)
    {
        addLandscape(landscape);
        return;
    }
    
    DynamicMapObject* dynamicMapObject = dynamic_cast<DynamicMapObject*>(child);
    if(dynamicMapObject)
    {
        addDynamicMapObject(dynamicMapObject);
        return;
    }
    
    BackgroundMapObject* backgroundMapObject = dynamic_cast<BackgroundMapObject*>(child);
    if(backgroundMapObject)
    {
        addBackgroundMapObject(backgroundMapObject);
        return;
    }
    
    Entity::addChild(child);
}