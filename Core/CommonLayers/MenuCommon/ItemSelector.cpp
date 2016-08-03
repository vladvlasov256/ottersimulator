//
//  ItemSelector.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#include "global.h"
#include "colors.h"
#include "strings.h"
#include "ItemSelector.hpp"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

bool ItemSelector::init()
{
    if ( !Widget::init() )
    {
        return false;
    }
    
    title = Label::create();
    title->setSystemFontSize(MENU_BUTTON_TITLE_FONT_SIZE);
//    title->setTextColor(MENU_TITLE_COLOR);
    addChild(title);
    
    previousButton = Button::create();
    previousButton->setTitleText("<");//!!!
    previousButton->setTouchEnabled(true);
    previousButton->setTitleFontSize(MENU_BUTTON_TITLE_FONT_SIZE);
    previousButton->getTitleRenderer()->enableShadow();
    
    previousButton->addTouchEventListener(CC_CALLBACK_2(ItemSelector::onPreviousButtonTouch, this));
    
    addChild(previousButton);
    
    nextButton= Button::create();
    nextButton->setTitleText(">");//!!!
    nextButton->setTouchEnabled(true);
    nextButton->setTitleFontSize(MENU_BUTTON_TITLE_FONT_SIZE);
    nextButton->getTitleRenderer()->enableShadow();
    
    nextButton->addTouchEventListener(CC_CALLBACK_2(ItemSelector::onNextButtonTouch, this));
    
    addChild(nextButton);
    
    return true;
}

void ItemSelector::setDelegate(const ItemSelectorDelegate *delegate)
{
    this->delegate = const_cast<ItemSelectorDelegate*>(delegate);
    
    updateButtonStates();
    updateTitle();
}

void ItemSelector::setTitleWidth(const float width)
{
    titleWidth = width;
    
    updatePositions();
}

void ItemSelector::setYPosition(const float yPos)
{
    float originX = Director::getInstance()->getWinSize().width * .5f;
    setPosition(Vec2(originX, yPos));
    
    updatePositions();
}

void ItemSelector::update()
{
    updateButtonStates();
    updateTitle();
}

void ItemSelector::updatePositions()
{
    title->setPosition(Vec2());
    previousButton->setPosition(Vec2(-titleWidth * .5f - ITEM_SELECTOR_BUTTON_GAP - ITEM_SELECTOR_BUTTON_WIDTH * .5f , 0));
    nextButton->setPosition(Vec2(titleWidth * .5f + ITEM_SELECTOR_BUTTON_GAP + ITEM_SELECTOR_BUTTON_WIDTH * .5f, 0));
}

void ItemSelector::updateButtonStates()
{
    previousButton->setEnabled(delegate && delegate->getCurrentIndex() > 0);
    nextButton->setEnabled(delegate && delegate->hasNext());
}

void ItemSelector::updateTitle()
{
    if(delegate)
    {
        title->setString(delegate->getTitle());
    }
}

void ItemSelector::onPreviousButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        if(delegate)
        {
            delegate->previousItemDidSelected();
        }
        
        update();
    }
}

void ItemSelector::onNextButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        if(delegate)
        {
            delegate->nextItemDidSelected();
        }
        
        update();
    }
}