//
//  GameActionLayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 02.05.16.
//
//

#include "GameActionLayer.hpp"
#include "GameScreen.hpp"

USING_NS_CC;
using namespace ottersimulator;

bool GameActionLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    
    this->keyboardListener = EventListenerKeyboard::create();
    this->keyboardListener->onKeyPressed = CC_CALLBACK_2(GameActionLayer::onKeyPressed, this);
    this->keyboardListener->onKeyReleased = CC_CALLBACK_2(GameActionLayer::onKeyReleased, this);
    
    eventDispatcher->addEventListenerWithSceneGraphPriority(this->keyboardListener, this);
    
    this->touchListener = EventListenerTouchOneByOne::create();
    this->touchListener->setSwallowTouches(true);
    this->touchListener->onTouchBegan = CC_CALLBACK_2(GameActionLayer::onTouchBegan, this);
    this->touchListener->onTouchMoved = CC_CALLBACK_2(GameActionLayer::onTouchMoved, this);
    this->touchListener->onTouchEnded = CC_CALLBACK_2(GameActionLayer::onTouchEnded, this);
    this->touchListener->onTouchCancelled = CC_CALLBACK_2(GameActionLayer::onTouchEnded, this);
    
    eventDispatcher->addEventListenerWithSceneGraphPriority(this->touchListener, this);
    
    return true;
}

void GameActionLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    auto gameScreen = static_cast<GameScreen*>(getScene());
    
    switch(keyCode)
    {
        case EventKeyboard::KeyCode::KEY_SPACE:
            gameScreen->onPushButtonTouchBegin();
            break;
        default:
            break;
    }
}

void GameActionLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    auto gameScreen = static_cast<GameScreen*>(getScene());
    
    switch(keyCode)
    {
        case EventKeyboard::KeyCode::KEY_SPACE:
            gameScreen->onPushButtonTouchEnd();
            break;
        case EventKeyboard::KeyCode::KEY_X:
            gameScreen->onChangeEnableDebugDrawPhysics();
            break;
        default:
            break;
    }
}

bool GameActionLayer::onTouchBegan(Touch* touch, Event* pEvent)
{
    if(touch->getLocation().x > Director::getInstance()->getWinSize().width * .5f)
    {
        return true;
    }
    
    return false;
}

void GameActionLayer::onTouchMoved(Touch* touch, Event* pEvent)
{
    if(touch->getLocation().x > Director::getInstance()->getWinSize().width * .5f)
    {
        static_cast<GameScreen*>(getScene())->getCharacter()->onTouchMoved(touch);
    }
}

void GameActionLayer::onTouchEnded(Touch* touch, Event* pEvent)
{
}
