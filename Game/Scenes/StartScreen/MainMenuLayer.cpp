//
//  MainMenuLayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#include "global.h"
#include "colors.h"
#include "strings.h"
#include "MainMenuLayer.hpp"
#include "StartScreen.hpp"
#include "LevelManager.hpp"
#include "AppDelegate.h"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

bool MainMenuLayer::init()
{
    if ( !MenuLayer::init() )
    {
        return false;
    }
    
    createTitle(MAIN_MENU_TITLE);
    
    if(LevelManager::getInstance()->getNumLevels())
    {
        addMenuButton(MAIN_MENU_GAME, CC_CALLBACK_2(MainMenuLayer::onGameButtonTouch, this));
        addMenuButton(MAIN_MENU_MULTIPLAYER, CC_CALLBACK_2(MainMenuLayer::onMultiplayerButtonTouch, this));
    }    
    
    return true;
}

void MainMenuLayer::onGameButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((StartScreen*)getScene())->presentLevelSelectionLayer();
    }
}

void MainMenuLayer::onMultiplayerButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((AppDelegate*)Application::getInstance())->replaceSceneWithMultiplayerScreen();
    }
}