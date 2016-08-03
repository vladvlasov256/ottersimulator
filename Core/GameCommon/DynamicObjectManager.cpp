//
//  DynamicObjectManager.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 31.05.16.
//
//

#include "global.h"
#include <string>
#include <vector>
#include "FileIOHelper.h"
#include "ResourceManager.hpp"
#include "DynamicObjectManager.hpp"
#include "JSONHelper.hpp"

USING_NS_CC;
using namespace ottersimulator;

DynamicObjectManager::~DynamicObjectManager()
{
    std::for_each(this->dynamicObjects.begin(), this->dynamicObjects.end(), [](DynamicObject* dynamicObject)
                  {
                      dynamicObject->release();
                  });
    this->dynamicObjects.clear();
}

static DynamicObjectManager *_instanceDynamicObjectManager = nullptr;

DynamicObjectManager* DynamicObjectManager::getInstance()
{
    if (_instanceDynamicObjectManager == nullptr)
    {
        _instanceDynamicObjectManager = new (std::nothrow) DynamicObjectManager();
    }
    return _instanceDynamicObjectManager;
}

void DynamicObjectManager::destroyInstance()
{
    CC_SAFE_DELETE(_instanceDynamicObjectManager);
}

void DynamicObjectManager::loadObjects()
{
    size_t nDynamicObjects = ResourceManager::getInstance()->getNumResources(ResourceManager::ResourceType::kDynamicObjects);
    this->dynamicObjects.reserve(nDynamicObjects);
    
    for(size_t objectIndex = 0; objectIndex < nDynamicObjects; objectIndex++)
    {
        auto dynamicObject = DynamicObject::createWithFileName(ResourceManager::getInstance()->getDynamicObjectFilePath(objectIndex));
        dynamicObject->retain();
        this->dynamicObjects.push_back(dynamicObject);
    }
}

DynamicObject* DynamicObjectManager::getByFileName(const std::string& fileName) const
{
    auto objectIt = std::find_if(this->dynamicObjects.begin(), this->dynamicObjects.end(), [&fileName](const DynamicObject* dynamicObject)
                                 {
                                     return fileName == dynamicObject->getFileName();
                                 });
    return (objectIt == this->dynamicObjects.end()) ? nullptr : *objectIt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DynamicObject* DynamicObject::createWithFileName(const std::string& fileName)
{
    DynamicObject *dynamicObject = new (std::nothrow) DynamicObject();
    if (dynamicObject && dynamicObject->initWithFileName(fileName))
    {
        dynamicObject->autorelease();
        return dynamicObject;
    }
    CC_SAFE_DELETE(dynamicObject);
    return nullptr;
}

bool DynamicObject::initWithFileName(const std::string& fileName)
{
    this->fileName = fileName;
    
    rapidjson::Document json;
    char* buffer;
    
    std::string filePath = GetPlatformPath(fileName);
    LoadJSONFromFile(filePath, json, buffer);
    
    this->textureFilePath = json["imagePath"].GetString();
    
    const rapidjson::Value& polygons = json["polygons"];
    for(rapidjson::SizeType i = 0; i < polygons.Size(); i++)
    {
        const rapidjson::Value& vertices = polygons[i];
        Polygon polygon;
        
        for(rapidjson::SizeType j = 0; j < vertices.Size(); j++)
        {
            const rapidjson::Value& vertex = vertices[j];
            polygon.push_back(Vec2(vertex["x"].GetDouble(), vertex["y"].GetDouble()));
            printf("%d_%d (%.3f, %.3f)\n", i, j, polygon.back().x, polygon.back().y);//!!!delete
        }
        
        this->polygons.push_back(polygon);
    }
    
    free(buffer);
    
    return true;
}