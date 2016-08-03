//
//  EditorTabMenu.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 24.04.16.
//
//

#ifndef EditorTabMenuLayer_hpp
#define EditorTabMenuLayer_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "ItemSelector.hpp"

namespace ottersimulator
{
    class EditorTabMenuDelegate
    {
    public:
        virtual size_t getTabNum() { return 0; }
        virtual cocos2d::ui::Widget* getThumbnailForTabBarButton(size_t tabIndex) { return NULL; }
        virtual size_t getItemNumForTab(size_t tabIndex) { return 0; }
        virtual cocos2d::ui::Widget* getThumbnailForItem(size_t tabIndex, size_t itemIndex) { return NULL; }
        virtual void onItemDidSelected(size_t tabIndex, size_t itemIndex) {}
    };
    
    class EditorTabBarButton : public cocos2d::ui::Widget
    {
    private:
        cocos2d::Color4F backgroundColor;
        cocos2d::DrawNode* backgroundNode;
        cocos2d::ui::Widget* thumbnail;
    protected:
        void invalidate();
    public:
        virtual bool init();
        void setBackgroundColor(const cocos2d::Color4F& color);
        void setThumbnail(cocos2d::ui::Widget* thumbnail);
        void setPosition(const cocos2d::Vec2 &pos);
        void setContentSize(const cocos2d::Size& contentSize);
        
        CREATE_FUNC(EditorTabBarButton);
    };
    
    class EditorTabMenu : public cocos2d::ui::Widget
    {
    private:
        std::vector<EditorTabBarButton*> tabButtons;
        std::vector<cocos2d::ui::Widget*> items;
        EditorTabMenuDelegate* delegate;
        size_t selectedIndex;
    protected:
        void invalidate();
        void onTabBarButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        void onTabBarItemTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
    public:
        virtual bool init();
        
        EditorTabMenuDelegate* getDelegate() const { return delegate; }
        void setDelegate(EditorTabMenuDelegate* delegate);
        
        void setPosition(const cocos2d::Vec2 &pos);
        void setContentSize(const cocos2d::Size& contentSize);
        
        CREATE_FUNC(EditorTabMenu);
    };
}

#endif /* EditorTabMenu_hpp */
