//
//  Entity.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#include "Entity.hpp"
#include "Landscape.hpp"
#include "BackgroundMapObject.hpp"
#include "DynamicMapObject.hpp"
#include "Map.hpp"
#include "JSONHelper.hpp"

USING_NS_CC;
using namespace ottersimulator;

bool Entity::init()
{
    if(!Node::init())
    {
        return false;
    }
    
    setName("");
    typeName = "Entity";
    
    return true;
}

Entity* Entity::LoadFromFile(const std::string& fileName)
{
    rapidjson::Document json;
    char* buffer;
    LoadJSONFromFile(fileName, json, buffer);
    
    Entity *entity = LoadFromJSON(json);
    
    free(buffer);
    
    return entity;
}

Entity* Entity::LoadFromJSON(const rapidjson::Value& json)
{
    Entity *entity = NULL;
    
    const std::string typeName = json["type"].GetString();
    if(typeName == "Entity")
    {
        entity = Entity::create();
    }
    else if(typeName == "BackgroundMapObject")
    {
        entity = BackgroundMapObject::create();
    }
    else if(typeName == "DynamicMapObject")
    {
        entity = DynamicMapObject::create();
    }
    else if(typeName == "Landscape")
    {
        entity = Landscape::create();
    }
    else if(typeName == "Map")
    {
        entity = ottersimulator::Map::create();
    }
    
    if(entity)
    {
        entity->loadImpl(json);
    }
    
    return entity;
}

void Entity::SaveToFile(const std::string& fileName)
{
    rapidjson::Document document;
    document.SetObject();
    
    SaveToJSON(document, document);
    
    SaveJSONToFile(fileName, document);
}

void Entity::SaveToJSON(rapidjson::Document& document, rapidjson::Value& json)
{
    WriteString(document, json, "type", getTypeName());
    saveImpl(document, json);
}

bool Entity::loadImpl(const rapidjson::Value& json)
{
    setName(json["name"].GetString());
    
    setPosition(ParseVec2(json["position"]));
    
    Vec2 scale = ParseVec2(json["scale"]);
    setScale(scale.x, scale.y);
    
    //what if we don't have them?
    const rapidjson::Value& children = json["children"];
    for(rapidjson::SizeType i = 0; i < children.Size(); i++)
    {
        Entity *child = LoadFromJSON(children[i]);
        addChild(child);
    }
    
    return true;
}

bool Entity::saveImpl(rapidjson::Document& document, rapidjson::Value& json)
{
    WriteString(document, json, "name", getName());
    WriteVec2(document, json, "position", getPosition());
    WriteVec2(document, json, "scale", Vec2(getScaleX(), getScaleY()));
    
    rapidjson::Value childrenValue(rapidjson::kArrayType);
    for(const auto& child : getChildren())
    {
        Entity* entity = dynamic_cast<Entity*>(child);
        if(entity)
        {
            rapidjson::Value childValue(rapidjson::kObjectType);
            ((Entity*)child)->SaveToJSON(document, childValue);
            childrenValue.PushBack(childValue, document.GetAllocator());
        }
    }
    json.AddMember("children", childrenValue, document.GetAllocator());
    
    return true;
}

bool Entity::isTouched(const Vec2& pos) const
{
    return getBounds().containsPoint(pos);
}

void Entity::setSize(const Size& size)
{
    this->size = size;
    this->updateBounds();
}

void Entity::updateBounds()
{
    bounds = Rect(getPosition() - Vec2(size) * .5f, size);
}

void Entity::setPosition(const cocos2d::Vec2 &pos)
{
    Node::setPosition(pos);
    this->updateBounds();
}