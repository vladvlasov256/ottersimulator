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
        MenuLayer* mainMenuLayer;
        MenuLayer* levelSelectionMenuLayer;
        std::stack<MenuLayer*> presentedMenus;
        cocos2d::Node* background;
        cocos2d::Layer* backgroundLayer;
    public:
        virtual bool init();
        
        void presentMainMenuLayer();
        void presentLevelSelectionLayer();
        
        void popMenuLayer();
        
        void setBackground(const std::string& image);
        
        CREATE_FUNC(StartScreen);
    };
}

#endif /* StartScreen_hpp */
