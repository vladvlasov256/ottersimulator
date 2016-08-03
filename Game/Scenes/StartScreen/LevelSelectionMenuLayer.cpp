//
//  LevelSelectionMenuLayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#include "global.h"
#include "colors.h"
#include "strings.h"
#include "LevelSelectionMenuLayer.hpp"
#include "LevelManager.hpp"
#include "StartScreen.hpp"
#include "AppDelegate.h"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

//!!!section selector

bool LevelSelectionMenuLayer::init()
{
    if ( !MenuLayer::init() )
    {
        return false;
    }
    
//    createTitle(LEVEL_SELECTION_TITLE);
    
//    levelSelector = addItemSelector(LEVEL_TITLE_WIDTH, this);
//    addMenuButton(LEVEL_SELECTION_PLAY, CC_CALLBACK_2(LevelSelectionMenuLayer::onPlayButtonTouch, this));
    addBackButton();
    
    addSectionButton("StartScreen/Park.png", Point(Director::getInstance()->getWinSize().width * .5f - 20, Director::getInstance()->getWinSize().height * .5f - 20), CC_CALLBACK_2(LevelSelectionMenuLayer::onParkButtonTouch, this));
//    addSectionButton("StartScreen/Mountains.png", Point(Director::getInstance()->getWinSize().width * .5f - 20, Director::getInstance()->getWinSize().height * .5f - 20), CC_CALLBACK_2(LevelSelectionMenuLayer::onParkButtonTouch, this));
//    addSectionButton("StartScreen/Forest.png", Point(Director::getInstance()->getWinSize().width * .5f - 20, Director::getInstance()->getWinSize().height * .5f - 20), CC_CALLBACK_2(LevelSelectionMenuLayer::onParkButtonTouch, this));
    
    return true;
}

void LevelSelectionMenuLayer::onMenuDidPresented()
{
//    currentIndex = 0;
//    levelSelector->update();
    ((StartScreen*)getScene())->setBackground("StartScreen/SinglePlayerGameBackground.jpg");
}

void LevelSelectionMenuLayer::addSectionButton(const std::string& buttonImage, const Vec2& position, const Widget::ccWidgetTouchCallback& callback)
{
    auto button = Button::create(buttonImage, buttonImage);
    button->setTouchEnabled(true);
    button->setPosition(position);
    button->setScale(Director::getInstance()->getContentScaleFactor() * .5f);
    
    button->addTouchEventListener(callback);
    
    this->addChild(button);
}

void LevelSelectionMenuLayer::onParkButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    ((AppDelegate*)Application::getInstance())->replaceSceneWithGameScreen(0);//!!!!
}

void LevelSelectionMenuLayer::onPlayButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
//        ((AppDelegate*)Application::getInstance())->replaceSceneWithGameScreen(0);//!!!!
    }
}

//bool LevelSelectionMenuLayer::hasNext() const
//{
//    return (currentIndex < LevelManager::getInstance()->getNumLevels() - 1);
//}
//
//std::string LevelSelectionMenuLayer::getTitle() const
//{
//    return std::string("Level ") + std::to_string(currentIndex + 1); //!!!!
//}