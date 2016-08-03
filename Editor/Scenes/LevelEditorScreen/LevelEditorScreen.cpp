//
//  LevelEditorScreen.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#include "global.h"
#include "LevelEditorScreen.hpp"
#include "LevelManager.hpp"
#include "Editor/AppDelegate.h"
#include "BackgroundMapObject.hpp"
#include "DynamicMapObject.hpp"

USING_NS_CC;
using namespace ottersimulator;

bool LevelEditorScreen::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    actionLayer = LevelEditorActionLayer::create();
    addChild(actionLayer);
    
    guiLayer = LevelEditorGUILayer::create();
    addChild(guiLayer);
    
    menuLayer = LevelEditorMenuLayer::create();
    menuLayer->retain();
    
    zoom = 1.f;
    
    return true;
}

void LevelEditorScreen::CreateNewMap()
{
    map = Map::create();
//    map->retain();
    
    map->createFlatLandscape();
    
    addMap();
}

void LevelEditorScreen::LoadMap(size_t mapIndex)
{
    this->mapIndex = mapIndex;
    
    map = (ottersimulator::Map*)Entity::LoadFromFile(LevelManager::getInstance()->getLevelFilePath(mapIndex));
    addMap();
}

void LevelEditorScreen::presentEditorMenuLayer()
{
    guiLayer->setVisible(false);
    actionLayer->setVisible(false);
    
    addChild(menuLayer);
    menuLayer->release();
}

void LevelEditorScreen::saveMapAndContinueEditing()
{
    menuLayer->retain();
    removeChild(menuLayer);
    
    saveMap();
    
    guiLayer->setVisible(true);
    actionLayer->setVisible(true);
}

void LevelEditorScreen::saveMapAndPlay()
{
    saveMap();
    
    ((AppDelegate*)Application::getInstance())->replaceSceneWithGameScreen(this->mapIndex);
}

void LevelEditorScreen::saveMapAndExit()
{
    saveMap();
    
    ((AppDelegate*)Application::getInstance())->replaceSceneWithStartScreen();
}

void LevelEditorScreen::exitWithouSaving()
{
    ((AppDelegate*)Application::getInstance())->replaceSceneWithStartScreen();
}

void LevelEditorScreen::saveMap()
{
    std::string filePath;
    
    if(map->getName().empty())
    {
        filePath = LevelManager::getInstance()->getNewLevelFilePath(this->mapIndex);
        
        std::string::size_type extensionBegin = filePath.find_last_of(".");
        std::string::size_type fileNameBegin = filePath.find_last_of("/") + 1;
        std::string fileName = filePath.substr(fileNameBegin, extensionBegin - fileNameBegin);
        
        map->setName(fileName);
    }
    else
    {
        filePath = LevelManager::getInstance()->getLevelFilePath(mapIndex);
    }
    
    map->SaveToFile(filePath);
}

void LevelEditorScreen::addMap()
{
    addChild(map, -1);
    
    auto landscape = this->map->getLandscape();
    if(landscape)
    {
        Point windowSize = Director::getInstance()->getWinSize();
        float xPos = landscape->getBounds().getMinX() + windowSize.x / windowSize.y * .5f * UNITS_IN_SCREEN_HEIGHT;
        this->focus = Vec2(xPos, landscape->getHeightAtPoint(xPos));
    }
    
    updateVisibleRect();
}

void LevelEditorScreen::updateVisibleRect()
{
    Point windowSize = Director::getInstance()->getWinSize();
    Vec2 visibleRectSize = Vec2(windowSize.x / windowSize.y, 1) * UNITS_IN_SCREEN_HEIGHT / zoom;
    Rect rect(focus.x - visibleRectSize.x * .5f, focus.y - visibleRectSize.y * .5f, visibleRectSize.x, visibleRectSize.y);
    map->setVisibleRect(rect);
    
    actionLayer->update();
}

void LevelEditorScreen::moveVisibleRect(const Vec2& offset)
{
    Point windowSize = Director::getInstance()->getWinSize();
    focus += Vec2(offset.x, offset.y) / windowSize.y * .5f * UNITS_IN_SCREEN_HEIGHT / zoom;
    updateVisibleRect();
}

void LevelEditorScreen::makeZoom(const float zoomDelta)
{
    float newZoom = zoom * ((zoomDelta < 0) ? .8f : 1.2f);
    if(newZoom >= .01f && newZoom <= 100.f)
    {
        zoom = newZoom;
        updateVisibleRect();
    }
}

void LevelEditorScreen::cretateBackgroundMapObject(const std::string& fileName)
{
    auto backgroundMapObject = map->addBackgroundMapObject(fileName);
    
    Vec2 position = focus;
    
    if(map->getLandscape())
    {
        float yPos = map->getLandscape()->getHeightAtPoint(focus.x);//!!!method
        position.y = isnan(yPos) ? position.y : (yPos + backgroundMapObject->getSize().height * .5f);
    }
    
    backgroundMapObject->setPosition(position);
}

void LevelEditorScreen::cretateDynamicMapObject(const std::string& fileName)
{
    auto dynamicMapObject = map->addDynamicMapObject(fileName);
    
    Vec2 position = focus;
    
    if(map->getLandscape())
    {
        float yPos = map->getLandscape()->getHeightAtPoint(focus.x);//!!!method
        position.y = isnan(yPos) ? position.y : (yPos + dynamicMapObject->getSize().height * .5f);
    }
    
    dynamicMapObject->setPosition(position);
}

void LevelEditorScreen::enableControlPointEditing()
{
    actionLayer->enableControlPointEditing();
}
void LevelEditorScreen::addBlockToTheLeft()
{
    auto landscape = this->map->getLandscape();
    if(landscape)
    {
        landscape->addFlatBlockToTheLeft();
        this->actionLayer->updateControlPointMarks();
    }
}

void LevelEditorScreen::addBlockToTheRight()
{
    auto landscape = this->map->getLandscape();
    if(landscape)
    {
        landscape->addFlatBlockToTheRight();
        this->actionLayer->updateControlPointMarks();
    }
}