//
//  ResourceManager.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 24.04.16.
//
//

#include "global.h"
#include <string>
#include <vector>
#include "FileIOHelper.h"
#include "ResourceManager.hpp"

using namespace ottersimulator;

ResourceManager::ResourceManager() : Ref()
{
    
}

ResourceManager::~ResourceManager()
{
    
}

static ResourceManager *_instanceResourceManager = nullptr;

ResourceManager* ResourceManager::getInstance()
{
    if (_instanceResourceManager == nullptr)
    {
        _instanceResourceManager = new (std::nothrow) ResourceManager();
    }
    return _instanceResourceManager;
}

void ResourceManager::destroyInstance()
{
    CC_SAFE_DELETE(_instanceResourceManager);
}

void ResourceManager::loadResources()
{
    loadStaticObjects();
    loadDynamicObjects();
    loadBackgrounds();
}

size_t ResourceManager::getNumResources(const ResourceType resourceType) const
{
    switch (resourceType)
    {
        case kStaticObjects:
            return staticObjectFilePathes.size();
        case kBackgrounds:
            return backgroundFilePathes.size();
        case kDynamicObjects:
            return dynamicObjectFilePathes.size();
    }
    
    return 0;
}

void ResourceManager::loadStaticObjects()
{
    std::vector<std::string> filePathes;
    GetFiles("StaticObjects", filePathes);
    setLocalPathes(filePathes, "StaticObjects", staticObjectFilePathes);
}

void ResourceManager::loadDynamicObjects()
{
    std::vector<std::string> filePathes;
    GetFiles("DynamicObjects", ".cosdo", filePathes);
    setLocalPathes(filePathes, "DynamicObjects", this->dynamicObjectFilePathes);
}

void ResourceManager::loadBackgrounds()
{
    std::vector<std::string> filePathes;
    GetFiles("Backgrounds", filePathes);
    setLocalPathes(filePathes, "Backgrounds", backgroundFilePathes);
}

void ResourceManager::setLocalPathes(const std::vector<std::string>& filePathes, const std::string& folder, std::vector<std::string>& localPathes)
{
    localPathes.clear();
    
    for(auto filePath : filePathes)
    {
        auto dirIndex = filePath.find(std::string("/") + folder);
        localPathes.push_back(filePath.substr(dirIndex + 1));
    }
}