//
//  EditorLevelSelectionLayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#ifndef EditorLevelSelectionLayer_hpp
#define EditorLevelSelectionLayer_hpp

#include "MenuLayer.hpp"

namespace ottersimulator
{
    class EditorLevelSelectionLayer : public MenuLayer, public ItemSelectorDelegate
    {
    private:
        ItemSelector* levelSelector;
    protected:
        void onEditLevelButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchEventType);
        
        virtual bool hasNext() const;
        virtual std::string getTitle() const;
        
        void onMenuDidPresented();
    public:
        virtual bool init();
        
        CREATE_FUNC(EditorLevelSelectionLayer);
    };
}

#endif /* EditorLevelSelectionLayer_hpp */
