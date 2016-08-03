//
//  ItemSelector.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#ifndef ItemSelector_hpp
#define ItemSelector_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"

namespace ottersimulator
{
    class ItemSelectorDelegate
    {
    protected:
        size_t currentIndex;
    public:
        virtual bool hasNext() const { return false; }
        virtual std::string getTitle() const { return ""; }
        size_t getCurrentIndex() const { return currentIndex; };
        virtual void previousItemDidSelected() { currentIndex--; }
        virtual void nextItemDidSelected() { currentIndex++; }
    };

    class ItemSelector : public cocos2d::ui::Widget
    {
    private:
        ItemSelectorDelegate *delegate;
        cocos2d::ui::Button* previousButton;
        cocos2d::ui::Button* nextButton;
        cocos2d::Label* title;
        float titleWidth;
        
        void updateButtonStates();
        void updateTitle();
        void updatePositions();
    protected:
        void onPreviousButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        void onNextButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
    public:
        virtual bool init();
        
        void setTitleWidth(const float width);
        void setYPosition(const float yPos);
        void update();
        
        ItemSelectorDelegate* getDelegate() const { return delegate; }
        void setDelegate(const ItemSelectorDelegate* delegate);
        
        CREATE_FUNC(ItemSelector);
    };
}

#endif /* ItemSelector_hpp */
