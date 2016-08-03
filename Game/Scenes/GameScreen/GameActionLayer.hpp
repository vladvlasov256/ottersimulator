//
//  GameActionLayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 02.05.16.
//
//

#ifndef GameActionLayer_hpp
#define GameActionLayer_hpp

#include "cocos2d.h"

namespace ottersimulator
{
    class GameActionLayer : public cocos2d::Layer
    {
    private:
        cocos2d::EventListenerKeyboard* keyboardListener;
        cocos2d::EventListenerTouchOneByOne* touchListener;
    protected:
        void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
        void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
        virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
        virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
        virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
    public:
        virtual bool init() override;
        
        CREATE_FUNC(GameActionLayer);
    };
}

#endif /* GameActionLayer_hpp */
