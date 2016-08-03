//
//  LevelEditorGUILayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#ifndef LevelEditorGUILayer_hpp
#define LevelEditorGUILayer_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "EditorTabMenu.hpp"

namespace ottersimulator
{
    class LevelEditorGUILayer : public cocos2d::Layer, public EditorTabMenuDelegate
    {
    private:
        EditorTabMenu* tabMenu;
//        std::vector<cocos2d::ui::Widget*> tabBarButtons;
        std::vector<std::string> tabBarButtonResources;
        //        std::vector<cocos2d::ui::Widget*> landscapeToolButtons;
        std::vector<std::string> landscapeToolButtonResources;
    protected:
        void onMenuButtonTouch(Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        
        size_t getTabNum() { return 4; }
        cocos2d::ui::Widget* getThumbnailForTabBarButton(size_t tabIndex);
        size_t getItemNumForTab(size_t tabIndex);
        cocos2d::ui::Widget* getThumbnailForItem(size_t tabIndex, size_t itemIndex);
        void onItemDidSelected(size_t tabIndex, size_t itemIndex);
    public:
        virtual bool init();
        
        CREATE_FUNC(LevelEditorGUILayer);
    };
}

#endif /* LevelEditorGUILayer_hpp */
