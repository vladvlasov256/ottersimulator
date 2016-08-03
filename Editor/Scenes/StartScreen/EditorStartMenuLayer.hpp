//
//  EditorStartMenu.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#ifndef EditorStartMenuLayer_hpp
#define EditorStartMenuLayer_hpp

#include "MenuLayer.hpp"

namespace ottersimulator
{
    class EditorStartMenuLayer : public MenuLayer
    {
    public:
        virtual bool init();
        
        void onNewLevelButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        void onEditLevelButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        
        CREATE_FUNC(EditorStartMenuLayer);
    };
}

#endif /* EditorStartMenuLayer_hpp */
