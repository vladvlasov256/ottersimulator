//
//  LevelEditorScreen.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#ifndef LevelEditorScreen_hpp
#define LevelEditorScreen_hpp

#include "cocos2d.h"
#include "LevelEditorActionLayer.hpp"
#include "LevelEditorGUILayer.hpp"
#include "LevelEditorMenuLayer.hpp"
#include "Map.hpp"

namespace ottersimulator
{
    class LevelEditorScreen : public cocos2d::Scene
    {
    private:
        LevelEditorActionLayer* actionLayer;
        LevelEditorGUILayer* guiLayer;
        LevelEditorMenuLayer* menuLayer;
        Map* map;
        size_t mapIndex;
        cocos2d::Vec2 focus;
        float zoom;
    protected:
        void saveMap();
        void addMap();
        void updateVisibleRect();
    public:
        virtual bool init();
        
        void CreateNewMap();
        void LoadMap(size_t mapIndex);
        Map* getMap() const { return map; }
        
        void presentEditorMenuLayer();
        void saveMapAndContinueEditing();
        void saveMapAndPlay();
        void saveMapAndExit();
        void exitWithouSaving();
        
        void moveVisibleRect(const cocos2d::Vec2& offset);
        void makeZoom(const float zoomDelta);
        
        void cretateBackgroundMapObject(const std::string& fileName);
        void cretateDynamicMapObject(const std::string& fileName);
        
        void enableControlPointEditing();
        void addBlockToTheLeft();
        void addBlockToTheRight();
        
        CREATE_FUNC(LevelEditorScreen);
    };
}

#endif /* LevelEditorScreen_hpp */
