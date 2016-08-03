//
//  EditorStartMenu.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#include "global.h"
#include "colors.h"
#include "strings.h"
#include "EditorStartMenuLayer.hpp"
#include "StartScreen.hpp"
#include "AppDelegate.h"
#include "LevelManager.hpp"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

bool EditorStartMenuLayer::init()
{
    if ( !MenuLayer::init() )
    {
        return false;
    }
    
    createTitle(EDITOR_START_SCREEN_TITLE);
    
    addMenuButton(EDITOR_START_SCREEN_NEW_LEVEL, CC_CALLBACK_2(EditorStartMenuLayer::onNewLevelButtonTouch, this));
    
    if(LevelManager::getInstance()->getNumLevels())
    {
        addMenuButton(EDITOR_START_SCREEN_EDIT_LEVEL, CC_CALLBACK_2(EditorStartMenuLayer::onEditLevelButtonTouch, this));
    }
    
    return true;
}

void EditorStartMenuLayer::onNewLevelButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((AppDelegate*)Application::getInstance())->replaceSceneWithLevelEditorWithNewMap();
    }
}

void EditorStartMenuLayer::onEditLevelButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((StartScreen*)getScene())->presentEditorLevelSelectionLayer();
    }
}