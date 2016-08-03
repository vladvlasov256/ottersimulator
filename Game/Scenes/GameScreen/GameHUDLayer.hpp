//
//  GameHUDLayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 02.05.16.
//
//

#ifndef GameHUDLayer_hpp
#define GameHUDLayer_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "EditorTabMenu.hpp"

namespace ottersimulator
{
    class GameHUDLayer : public cocos2d::Layer
    {
    private:
        cocos2d::Sprite* pushIconAcitve;
        cocos2d::Sprite* pushIconInacitve;
        cocos2d::ProgressTimer* progressTimer;
    protected:
        void onPushButtonTouch(Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        
    public:
        virtual bool init();
        void setPushIconVisible(bool isVisible);
        void setPushIconProgress(float percentage);
        
        CREATE_FUNC(GameHUDLayer);
    };
}

#endif /* GameHUDLayer_hpp */
