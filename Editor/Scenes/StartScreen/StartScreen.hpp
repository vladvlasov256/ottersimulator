//
//  StartScreen.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#ifndef StartScreen_hpp
#define StartScreen_hpp

#include "cocos2d.h"
#include "MenuLayer.hpp"

namespace ottersimulator
{
    class StartScreen : public cocos2d::Scene
    {
    private:
        MenuLayer* editorStartMenuLayer;
        MenuLayer* editorLevelSelectionLayer;
        std::stack<MenuLayer*> presentedMenus;
        
        cocos2d::Layer* createBackgroundLayer();
    public:
        virtual bool init();
        
        void presentEditorLevelSelectionLayer();
        
        void popMenuLayer();
        
        CREATE_FUNC(StartScreen);
    };
}

#endif /* StartScreen_hpp */
