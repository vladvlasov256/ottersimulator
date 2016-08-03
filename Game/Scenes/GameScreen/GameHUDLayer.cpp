//
//  GameHUDLayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 02.05.16.
//
//

#include "GameHUDLayer.hpp"
#include "GameScreen.hpp"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

bool GameHUDLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto button = Button::create("Textures/GUI/Game/push.png", "Textures/GUI/Game/push_pressed.png");
    button->setTouchEnabled(true);
    button->setPosition(Point(Director::getInstance()->getWinSize().width * .05f, Director::getInstance()->getWinSize().height * .1f));
    button->setScale(Director::getInstance()->getContentScaleFactor() * .5f);
    
    button->addTouchEventListener(CC_CALLBACK_2(GameHUDLayer::onPushButtonTouch, this));
    
    addChild(button);
    
    Vec2 pushIconPosition = Vec2(Director::getInstance()->getWinSize().width * .05f, Director::getInstance()->getWinSize().height * .2f);
    this->pushIconAcitve = Sprite::create("Textures/GUI/Game/push.png");
    this->pushIconAcitve->setPosition(pushIconPosition);
    this->pushIconAcitve->setScale(Director::getInstance()->getContentScaleFactor() * .5f);
    this->pushIconAcitve->setVisible(false);
    this->pushIconAcitve->retain();
    
    addChild(this->pushIconAcitve);
    
    this->pushIconInacitve = Sprite::create("Textures/GUI/Game/push_progress.png");
    this->pushIconInacitve->setPosition(pushIconPosition);
    this->pushIconInacitve->setScale(Director::getInstance()->getContentScaleFactor() * .5f);
    this->pushIconInacitve->setVisible(false);
    this->pushIconAcitve->retain();
    
    addChild(this->pushIconInacitve);
    
    this->progressTimer = ProgressTimer::create(Sprite::create("Textures/GUI/Game/push_progress_active.png"));
    this->progressTimer->setPosition(pushIconPosition);
    this->progressTimer->setScale(Director::getInstance()->getContentScaleFactor() * .5f);
    this->progressTimer->setVisible(false);
    this->progressTimer->retain();
    
    addChild(this->progressTimer);
    
    return true;
}

void GameHUDLayer::onPushButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::BEGAN)
    {
        static_cast<GameScreen*>(getScene())->onPushButtonTouchBegin();
    }
    else if(touchEventType == Widget::TouchEventType::ENDED)
    {
        static_cast<GameScreen*>(getScene())->onPushButtonTouchEnd();
    }
}

void GameHUDLayer::setPushIconVisible(bool isVisible)
{
    this->pushIconInacitve->setVisible(isVisible);
    if(!isVisible)
    {
        this->pushIconAcitve->setVisible(false);
        this->progressTimer->setVisible(false);
    }
}

void GameHUDLayer::setPushIconProgress(float percentage)
{
    this->pushIconInacitve->setVisible(false);
    this->pushIconAcitve->setVisible(true);
    this->progressTimer->setVisible(true);
    this->progressTimer->setPercentage(percentage);
}