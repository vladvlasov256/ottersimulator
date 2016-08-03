//
//  LevelSelectionMenuLayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#ifndef LevelSelectionMenuLayer_hpp
#define LevelSelectionMenuLayer_hpp

#include "MenuLayer.hpp"

namespace ottersimulator
{
    class LevelSelectionMenuLayer : public MenuLayer /*, public ItemSelectorDelegate*/
    {
//    private:
//        ItemSelector* levelSelector;
    protected:
        void onPlayButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        void onParkButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        
        void addSectionButton(const std::string& buttonImage, const cocos2d::Vec2& position, const cocos2d::ui::Widget::ccWidgetTouchCallback& callback);
        
//        virtual bool hasNext() const;
//        virtual std::string getTitle() const;
        
        void onMenuDidPresented();
    public:
        virtual bool init();
        
        CREATE_FUNC(LevelSelectionMenuLayer);
    };
}

#endif /* LevelSelectionMenuLayer_hpp */
