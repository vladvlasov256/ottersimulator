//
//  LevelEditorMenuLayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#ifndef LevelEditorMenuLayer_hpp
#define LevelEditorMenuLayer_hpp

#include "MenuLayer.hpp"

namespace ottersimulator
{
    class LevelEditorMenuLayer : public MenuLayer
    {
    private:
        ItemSelector* levelSelector;
    protected:
        void onSaveAndContinueEditingButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        void onSaveAndPlayButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        void onSaveAndExitButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        void onExitWithoutSavingButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        
//        void onMenuDidPresented();
    public:
        virtual bool init();
        
        CREATE_FUNC(LevelEditorMenuLayer);
    };
}

#endif /* LevelEditorMenuLayer_hpp */
