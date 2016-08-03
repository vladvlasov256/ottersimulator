//
//  EditorTabMenu.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 24.04.16.
//
//

#include "EditorTabMenu.hpp"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

#define kSelectedTabColor Color4F(.25f, .25f, .25f, 1.f)
#define kDefaultTabColor Color4F(.8f, .8f, .8f, 1.f)
#define kSepartorColor Color4F(1.f, 1.f, 1.f, 1.f)

bool EditorTabMenu::init()
{
    if ( !Widget::init() )
    {
        return false;
    }
    
    return true;
}

void EditorTabMenu::setDelegate(EditorTabMenuDelegate* delegate)
{
    this->delegate = delegate;
    selectedIndex = 0;
    invalidate();
}

void EditorTabMenu::invalidate()
{
    tabButtons.clear();
    items.clear();
    
    removeAllChildren();
    
    if(delegate)
    {
        size_t tabNum = delegate->getTabNum();
        if(!tabNum)
        {
            return;
        }
        
        float xPos = 0;
        float yPos = getContentSize().height - 1;
        float tabButtonWidth = (getContentSize().width - (tabNum - 1) * 2) / tabNum;
        float height = 42; //!!!magic const
        
        for(size_t iTab = 0; iTab < tabNum; iTab++)
        {
            auto tabButton = EditorTabBarButton::create();
            tabButton->setPosition(Vec2(xPos + tabButtonWidth * .5f, yPos - height * .5f));
            tabButton->setContentSize(Size(tabButtonWidth, height));
            tabButton->setBackgroundColor((iTab == selectedIndex) ? kSelectedTabColor : kDefaultTabColor);
            tabButton->setThumbnail(delegate->getThumbnailForTabBarButton(iTab));
            tabButton->addTouchEventListener(CC_CALLBACK_2(EditorTabMenu::onTabBarButtonTouch, this));
            addChild(tabButton);
            tabButtons.push_back(tabButton);
            
            if(iTab < tabNum - 1)
            {
                auto separator = DrawNode::create();
                separator->setLineWidth(2);
                separator->drawRect(Vec2(xPos + tabButtonWidth, yPos - height + 1), Vec2(xPos + tabButtonWidth + 1, yPos - 1), kSepartorColor);
                addChild(separator);
            }
            
            xPos += tabButtonWidth + 2;
        }
        
        const Vec2 contentAreaVertices[4] = {Vec2(0, 0), Vec2(0, yPos - height), Vec2(getContentSize().width - 1, yPos - height), Vec2(getContentSize().width - 1, 0)};
        
        auto contentArea = DrawNode::create();
        contentArea->drawPolygon(contentAreaVertices, 4, kSelectedTabColor, 0, Color4F());
        addChild(contentArea);
        
        float itemHeight = getContentSize().height - 48 - 20;
        float itemYPos = 10 + itemHeight * .5f;
        
        size_t itemsOnSelectedTab = delegate->getItemNumForTab(selectedIndex);
        
        for(size_t iItem = 0; iItem < itemsOnSelectedTab; iItem++)
        {
            auto itemThumbnail = delegate->getThumbnailForItem(selectedIndex, iItem);
            itemThumbnail->setPosition(Vec2(10 * (iItem + 1) + itemHeight * (iItem + .5f), itemYPos));
            itemThumbnail->setScale(itemHeight / MAX(itemThumbnail->getContentSize().width, itemThumbnail->getContentSize().height));
            itemThumbnail->setTouchEnabled(true);
            itemThumbnail->addTouchEventListener(CC_CALLBACK_2(EditorTabMenu::onTabBarItemTouch, this));
            addChild(itemThumbnail);
            items.push_back(itemThumbnail);
        }

//        auto rect = DrawNode::create();
//        rect->drawRect(Vec2(0, 0), Vec2(getContentSize().width - 1, getContentSize().height - 1), Color4F(1, 0, 0, 1));
//        rect->setLineWidth(2);
//        addChild(rect);
    }
}

void EditorTabMenu::setPosition(const Vec2 &pos)
{
    Widget::setPosition(pos);
    invalidate();
}

void EditorTabMenu::setContentSize(const Size& contentSize)
{
    Widget::setContentSize(contentSize);
    invalidate();
}

void EditorTabMenu::onTabBarButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        size_t index = 0;
        for(auto it = tabButtons.begin(); it != tabButtons.end(); ++it, ++index)
        {
            if(*it == pSender)
            {
                selectedIndex = index;
                invalidate();
                return;
            }
        }
    }
}

void EditorTabMenu::onTabBarItemTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        size_t index = 0;
        for(auto it = items.begin(); it != items.end(); ++it, ++index)
        {
            if(*it == pSender)
            {
                if(delegate)
                {
                    delegate->onItemDidSelected(selectedIndex, index);
                }
                return;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool EditorTabBarButton::init()
{
    if ( !Widget::init() )
    {
        return false;
    }
    
    setTouchEnabled(true);
    
    return true;
}

void EditorTabBarButton::setBackgroundColor(const Color4F& color)
{
    backgroundColor = color;
    invalidate();
}

void EditorTabBarButton::setThumbnail(cocos2d::ui::Widget* thumbnail)
{
    if(this->thumbnail)
    {
        removeChild(this->thumbnail);
    }
    
    this->thumbnail = thumbnail;
    this->thumbnail->setTouchEnabled(false);
    
    invalidate();
}

void EditorTabBarButton::setPosition(const Vec2 &pos)
{
    Widget::setPosition(pos);
    invalidate();
}

void EditorTabBarButton::setContentSize(const Size& contentSize)
{
    Widget::setContentSize(contentSize);
    invalidate();
}

void EditorTabBarButton::invalidate()
{
    removeAllChildren();
    
    float width = getContentSize().width;
    float height = getContentSize().height;
    
    const Vec2 vertices[4] = {Vec2(0, -0), Vec2(0, height), Vec2(width - 1, height), Vec2(width - 1, 0)};
    
    backgroundNode = DrawNode::create();
    backgroundNode->drawPolygon(vertices, 4, backgroundColor, 0, Color4F());
    addChild(backgroundNode);
    
    if(thumbnail)
    {
        thumbnail->setPosition(Vec2(width * .5f, height * .5f));
        thumbnail->setContentSize(Size(32, 32));//!!!attribute
        addChild(thumbnail);
    }
}