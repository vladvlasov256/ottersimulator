//
//  LevelEditorActionLayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#include "global.h"
#include "LevelEditorActionLayer.hpp"
#include "LevelEditorScreen.hpp"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

bool LevelEditorActionLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->touchListener = EventListenerTouchOneByOne::create();
    this->touchListener->setSwallowTouches(true);
    this->touchListener->onTouchBegan = CC_CALLBACK_2(LevelEditorActionLayer::onTouchBegan, this);
    this->touchListener->onTouchMoved = CC_CALLBACK_2(LevelEditorActionLayer::onTouchMoved, this);
    this->touchListener->onTouchEnded = CC_CALLBACK_2(LevelEditorActionLayer::onTouchEnded, this);
    this->touchListener->onTouchCancelled = CC_CALLBACK_2(LevelEditorActionLayer::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(this->touchListener, this);
    
    this->mouseListener = EventListenerMouse::create();
    this->mouseListener->onMouseScroll = CC_CALLBACK_1(LevelEditorActionLayer::onMouseScroll, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(this->mouseListener, this);
    
    return true;
}

bool LevelEditorActionLayer::onTouchBegan(Touch* touch, Event* pEvent)
{
    unschedule(CC_SCHEDULE_SELECTOR(LevelEditorActionLayer::longPressTimer));
    
    hasLongPressUsed = false;
    schedule(CC_SCHEDULE_SELECTOR(LevelEditorActionLayer::longPressTimer), .5f);
    
    scrollLastPosition = touch->getLocation();
    
    auto markIt = std::find_if(controlPointMarks.begin(), controlPointMarks.end(), [touch](LandscapeControlPointMark* mark)
                               {
                                   return mark->isTouched(touch->getLocation());
                               });
    
    if(markIt != controlPointMarks.end())
    {
        selectedControlPointEditMark = *markIt;
        selectedControlPointEditMark->setActive();
    }
    
    return true;
}

void LevelEditorActionLayer::onTouchMoved(Touch* touch, Event* pEvent)
{
    unschedule(CC_SCHEDULE_SELECTOR(LevelEditorActionLayer::longPressTimer));
    
    Vec2 currentScrollPosition = touch->getLocation();
    Vec2 offset = currentScrollPosition - scrollLastPosition;
    
    if(selectedEntity)
    {
        auto map = static_cast<LevelEditorScreen*>(getScene())->getMap();
        selectedEntity->setPosition(selectedEntity->getPosition() + offset / map->getScale());
    }
    else if(selectedControlPointEditMark)
    {
        selectedControlPointEditMark->setHeight(selectedControlPointEditMark->getPosition().y + offset.y);
    }
    else
    {
        hasLongPressUsed = false;
        static_cast<LevelEditorScreen*>(getScene())->moveVisibleRect(-offset);
    }
    
    scrollLastPosition = currentScrollPosition;
}

void LevelEditorActionLayer::onTouchEnded(Touch* touch, Event* pEvent)
{
    unschedule(CC_SCHEDULE_SELECTOR(LevelEditorActionLayer::longPressTimer));
    
    if(hasLongPressUsed && !selectedEntity)
    {
        disableControlPointEditing();
    }
    else if(selectedControlPointEditMark)
    {
        selectedControlPointEditMark->setInactive();
    }
    
    selectedControlPointEditMark = nullptr;
    selectedEntity = nullptr;
    hasLongPressUsed = false;
}

void LevelEditorActionLayer::onMouseScroll(EventMouse* event)
{
    static_cast<LevelEditorScreen*>(getScene())->makeZoom(.1f * event->getScrollY());
}

void LevelEditorActionLayer::enableControlPointEditing()
{
    disableControlPointEditing();
    
    auto map = static_cast<LevelEditorScreen*>(getScene())->getMap();
    
    if(map && map->getLandscape())
    {
        auto landscape = map->getLandscape();
        controlPointMarks.reserve(landscape->getControlPointForEditingCount());
        
        Size windowSize = Director::getInstance()->getWinSize();
        
        auto controlPointIt = landscape->getControlPointsForEditingBegin();
        auto controlPointEnd = landscape->getControlPointsForEditingEnd();
        for( ; controlPointIt != controlPointEnd; ++controlPointIt)
        {
            auto editMark = LandscapeControlPointMark::create(*controlPointIt);
            editMark->setScale(.5f);
            addChild(editMark);
            this->controlPointMarks.push_back(editMark);
            editMark->update();
        }
    }
    
    selectedControlPointEditMark = nullptr;
}

void LevelEditorActionLayer::disableControlPointEditing()
{
    selectedControlPointEditMark = nullptr;
    
    std::for_each(controlPointMarks.begin(), controlPointMarks.end(), [this](LandscapeControlPointMark* mark)
    {
        this->removeChild(mark);
    });
    controlPointMarks.clear();
}

void LevelEditorActionLayer::updateControlPointMarks()
{
    if(!this->controlPointMarks.empty())
    {
        this->enableControlPointEditing();
    }
}

void LevelEditorActionLayer::update()
{
    std::for_each(controlPointMarks.begin(), controlPointMarks.end(), [](LandscapeControlPointMark* mark)
    {
        mark->update();
    });
}

void LevelEditorActionLayer::longPressTimer(float dt)
{
    hasLongPressUsed = true;
    unschedule(CC_SCHEDULE_SELECTOR(LevelEditorActionLayer::longPressTimer));
    
    //!!!sync
    auto map = static_cast<LevelEditorScreen*>(getScene())->getMap();
    if(map)
    {
        selectedEntity = map->getTouchedObject((scrollLastPosition - map->getPosition()) / map->getScale());
    }
}

void LevelEditorActionLayer::setVisible(bool isVisible)
{
    this->touchListener->setEnabled(isVisible);
    this->mouseListener->setEnabled(isVisible);
    Layer::setVisible(isVisible);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LandscapeControlPointMark* LandscapeControlPointMark::create(ControlPoint* controlPoint)
{
    LandscapeControlPointMark *mark = new (std::nothrow) LandscapeControlPointMark();
    if (mark && mark->initWithControlPoint(controlPoint))
    {
        mark->autorelease();
        return mark;
    }
    CC_SAFE_DELETE(mark);
    return nullptr;
}

bool LandscapeControlPointMark::initWithControlPoint(ControlPoint* controlPoint)
{
    if(!Widget::init())
    {
        return false;
    }
    
//    button = Button::create("Textures/GUI/Editor/landscapeControlPointEditMark.png", "Textures/GUI/Editor/landscapeControlPointEditMarkActive.png");
//    addChild(button);
    sprite = Sprite::create("Textures/GUI/Editor/landscapeControlPointEditMark.png");
    addChild(sprite);
    
    activeSprite = Sprite::create("Textures/GUI/Editor/landscapeControlPointEditMarkActive.png");
    activeSprite->setVisible(false);
    addChild(activeSprite);
    
    this->controlPoint = controlPoint;
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(LandscapeControlPointMark::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LandscapeControlPointMark::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LandscapeControlPointMark::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void LandscapeControlPointMark::update()
{
    auto map = static_cast<LevelEditorScreen*>(getScene())->getMap();
    if(map && map->getLandscape())
    {
        Size windowSize = Director::getInstance()->getWinSize();
        setPosition(this->controlPoint->getPosition() * map->getScale() + map->getPosition());
    }
}

//bool LandscapeControlPointMark::onTouchBegan(Touch* touch, Event* pEvent)
//{
//    activeSprite->setVisible(true);
//    sprite->setVisible(false);
//    lastTouchPosition = touch->getLocation();
//    return true;
//}
//
//void LandscapeControlPointMark::onTouchMoved(Touch* touch, Event* pEvent)
//{
//    setPosition(getPosition() + lastTouchPosition - touch->getLocation());
//    lastTouchPosition = touch->getLocation();
//}
//
//void LandscapeControlPointMark::onTouchEnded(Touch* touch, Event* pEvent)
//{
//    activeSprite->setVisible(false);
//    sprite->setVisible((true));
//    setPosition(getPosition() + lastTouchPosition - touch->getLocation());
//    lastTouchPosition = touch->getLocation();
//}
//
//bool LandscapeControlPointMark::hitTest(const Vec2 &pt, const Camera* camera, Vec3 *p) const
//{
//    return false;
//}

bool LandscapeControlPointMark::isTouched(const cocos2d::Vec2& pos) const
{
    float radius = sprite->getContentSize().width * .5f * getScale();
    return (pos - getPosition()).length() <= radius;
}

void LandscapeControlPointMark::setActive()
{
    activeSprite->setVisible(true);
    sprite->setVisible(false);
}

void LandscapeControlPointMark::setInactive()
{
    activeSprite->setVisible(false);
    sprite->setVisible(true);
}

void LandscapeControlPointMark::setHeight(const float height)
{
    setPosition(Vec2(getPosition().x, height));
    
    auto map = static_cast<LevelEditorScreen*>(getScene())->getMap();
    if(map && map->getLandscape())
    {
        float controlPointHeight = (height - map->getPosition().y) / map->getScale();
        controlPoint->setHeight(controlPointHeight);
    }
}