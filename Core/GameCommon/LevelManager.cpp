
//  LevelManager.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#include "global.h"
#include <string>
#include <vector>
#include "FileIOHelper.h"
#include "LevelManager.hpp"

using namespace ottersimulator;

LevelManager::LevelManager() : Ref()
{
    
}

LevelManager::~LevelManager()
{
    
}

static LevelManager *_instanceLevelManager = nullptr;

LevelManager* LevelManager::getInstance()
{
    if (_instanceLevelManager == nullptr)
    {
        _instanceLevelManager = new (std::nothrow) LevelManager();
    }
    return _instanceLevelManager;
}

void LevelManager::destroyInstance()
{
    CC_SAFE_DELETE(_instanceLevelManager);
}

void LevelManager::loadLevels()
{
    std::string fullPath;
    
    //!!!change: platform -> target
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    fullPath = std::string(LEVEL_FOLDER_NAME);
#else
    std::string levelsDir = std::string(USER_FOLDER_NAME) + std::string("/") + std::string(LEVEL_FOLDER_NAME);
    GetUserDirectoryPath(levelsDir, fullPath);
#endif
    
    levelFilePathes.clear();
    GetFiles(fullPath, levelFilePathes);
}

size_t LevelManager::getNumLevels()
{
    return levelFilePathes.size();
}

const std::string& LevelManager::getLevelFilePath(size_t levelIndex) const
{
    return levelFilePathes[levelIndex];
}

const std::string& LevelManager::getNewLevelFilePath(size_t& levelIndex)
{
    levelIndex = getNumLevels();
    
    char mapName[32];
    snprintf(mapName, sizeof(mapName), "map_%02ld.cosm", levelIndex + 1);//!!!magic const
    
    std::string levelsDir = std::string(USER_FOLDER_NAME) + std::string("/") + std::string(LEVEL_FOLDER_NAME);
    std::string filePath = levelsDir + std::string("/") + std::string(mapName);
    
    std::string fullPath;
    GetUserDirectoryPath(filePath, fullPath);
    
    levelFilePathes.push_back(fullPath);
    
    return getLevelFilePath(levelIndex);
}