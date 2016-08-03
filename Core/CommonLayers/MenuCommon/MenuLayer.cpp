//
//  MenuLayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#include "global.h"
#include "colors.h"
#include "strings.h"
#include "MenuLayer.hpp"
#include "StartScreen.hpp"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

bool MenuLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

Label* MenuLayer::createTitle(const std::string& text)
{
    auto label = Label::createWithTTF(text, MENU_TITLE_FONT_NAME, MENU_TITLE_FONT_SIZE);
    label->setPosition(MENU_TITLE_POSITION);
//    label->setTextColor(MENU_TITLE_COLOR);
    label->enableOutline(MENU_TITLE_OUTLINE_COLOR, MENU_TITLE_OUTLINE_SIZE);
    addChild(label);
    
    return label;
}

Button* MenuLayer::addMenuButton(const std::string& text, const Button::ccWidgetTouchCallback& callback)
{
    auto button = Button::create();
    button->setTitleText(text);
    button->setTouchEnabled(true);
    button->setPosition(MENU_BUTTONS_ORIGIN - MENU_BUTTONS_GAP * items.size());
    button->setTitleFontSize(MENU_BUTTON_TITLE_FONT_SIZE);
    button->getTitleRenderer()->enableShadow();
    
    button->addTouchEventListener(callback);
    
    addChild(button);
    
    items.push_back(button);
    
    return button;
}

ItemSelector* MenuLayer::addItemSelector(const float titleWidth, const ItemSelectorDelegate* itemSelectorDelegate)
{
    auto itemSelector = ItemSelector::create();
    itemSelector->setYPosition((MENU_BUTTONS_ORIGIN - MENU_BUTTONS_GAP * items.size()).y);
    itemSelector->setTitleWidth(titleWidth);
    itemSelector->setDelegate(itemSelectorDelegate);
    
    items.push_back(itemSelector);
    
    addChild(itemSelector);
    
    return itemSelector;
}

void MenuLayer::addBackButton()
{
    if(!backButton)
    {
        backButton = Button::create("BackButton.png", "BackButton.png");
        backButton->setTouchEnabled(true);
        backButton->setPosition(BACK_BUTTON_ORIGIN);
        
        backButton->addTouchEventListener(CC_CALLBACK_2(MenuLayer::onBackButtonTouch, this));
        
        addChild(backButton);
    }
    
    backButton->setVisible(true);
}

void MenuLayer::onBackButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((StartScreen*)getScene())->popMenuLayer();
    }
}