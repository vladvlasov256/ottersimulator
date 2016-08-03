//
//  LevelEditorMenuLayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#include "global.h"
#include "colors.h"
#include "strings.h"
#include "LevelEditorMenuLayer.hpp"
#include "LevelEditorScreen.hpp"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

bool LevelEditorMenuLayer::init()
{
    if ( !MenuLayer::init() )
    {
        return false;
    }
    
    createTitle(LEVEL_EDITOR_MENU_TITLE);
    
    addMenuButton(LEVEL_EDITOR_MENU_SAVE_AND_CONTINUE_EDITING, CC_CALLBACK_2(LevelEditorMenuLayer::onSaveAndContinueEditingButtonTouch, this));
    addMenuButton(LEVEL_EDITOR_MENU_SAVE_AND_PLAY, CC_CALLBACK_2(LevelEditorMenuLayer::onSaveAndPlayButtonTouch, this));
    addMenuButton(LEVEL_EDITOR_MENU_SAVE_AND_EXIT, CC_CALLBACK_2(LevelEditorMenuLayer::onSaveAndExitButtonTouch, this));
    addMenuButton(LEVEL_EDITOR_MENU_EXIT_WITHOUT_SAVING, CC_CALLBACK_2(LevelEditorMenuLayer::onExitWithoutSavingButtonTouch, this));
    
    return true;
}

void LevelEditorMenuLayer::onSaveAndContinueEditingButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((LevelEditorScreen*)getScene())->saveMapAndContinueEditing();
    }
}

void LevelEditorMenuLayer::onSaveAndPlayButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((LevelEditorScreen*)getScene())->saveMapAndPlay();
    }
}

void LevelEditorMenuLayer::onSaveAndExitButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((LevelEditorScreen*)getScene())->saveMapAndExit();
    }
}

void LevelEditorMenuLayer::onExitWithoutSavingButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((LevelEditorScreen*)getScene())->exitWithouSaving();
    }
}