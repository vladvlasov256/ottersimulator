//
//  MenuLayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#ifndef MenuLayer_hpp
#define MenuLayer_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "ItemSelector.hpp"

namespace ottersimulator
{
    class MenuLayer : public cocos2d::Layer
    {
    private:
        std::vector<cocos2d::ui::Widget*> items;
        cocos2d::ui::Button* backButton;
    protected:
        cocos2d::Label* createTitle(const std::string& text);
        cocos2d::ui::Button* addMenuButton(const std::string& text, const cocos2d::ui::Button::ccWidgetTouchCallback& callback);
        ItemSelector* addItemSelector(const float titleWidth, const ItemSelectorDelegate* itemSelectorDelegate);
        void addBackButton();
        
        virtual void onBackButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
    public:
        virtual bool init();
        virtual void onMenuDidPresented() {};
    };
}

#endif /* MenuLayer_hpp */
