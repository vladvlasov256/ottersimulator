//
//  MainMenuLayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#ifndef MainMenuLayer_hpp
#define MainMenuLayer_hpp

#include "MenuLayer.hpp"

namespace ottersimulator
{
    class MainMenuLayer : public MenuLayer
    {
    public:
        virtual bool init();
        
        void onGameButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        void onMultiplayerButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        
        CREATE_FUNC(MainMenuLayer);
    };
}

#endif /* MainMenuLayer_hpp */
