//
//  EditorLevelSelectionLayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#include "global.h"
#include "colors.h"
#include "strings.h"
#include "EditorLevelSelectionLayer.hpp"
#include "LevelManager.hpp"
#include "StartScreen.hpp"
#include "AppDelegate.h"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

bool EditorLevelSelectionLayer::init()
{
    if ( !MenuLayer::init() )
    {
        return false;
    }
    
    createTitle(LEVEL_SELECTION_TITLE);
    
    levelSelector = addItemSelector(LEVEL_TITLE_WIDTH, this);
    addMenuButton(LEVEL_SELECTION_EDIT, CC_CALLBACK_2(EditorLevelSelectionLayer::onEditLevelButtonTouch, this));
    addBackButton();
    
    return true;
}

void EditorLevelSelectionLayer::onMenuDidPresented()
{
    currentIndex = 0;
    levelSelector->update();
}

void EditorLevelSelectionLayer::onEditLevelButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((AppDelegate*)Application::getInstance())->replaceSceneWithLevelEditor(currentIndex);
    }
}

bool EditorLevelSelectionLayer::hasNext() const
{
    return (currentIndex < LevelManager::getInstance()->getNumLevels() - 1);
}

std::string EditorLevelSelectionLayer::getTitle() const
{
    return std::string("Level ") + std::to_string(currentIndex + 1); //!!!!real Name?
}
