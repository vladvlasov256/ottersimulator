//
//  GameScreen.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#ifndef GameScreen_hpp
#define GameScreen_hpp

#include "cocos2d.h"
#include "Map.hpp"
#include "GameActionLayer.hpp"
#include "GameHUDLayer.hpp"
#include "Longboard.hpp"
#include "Character.hpp"
#include "external/Box2D/Box2D.h"//!!!headers path
#include "B2DebugDrawLayer.h"

namespace ottersimulator
{
    class GameScreen : public cocos2d::Scene
    {
    private:
        GameActionLayer* actionLayer;
        GameHUDLayer* hudLayer;
        size_t mapIndex;
        Map* map;
        cocos2d::Vec2 focus;
        float zoom;
        Longboard* longboard;
        Character* character;
        b2World* world;
        b2Body *landscapeBody;
        B2DebugDrawLayer* physicsDebugLayer;
        bool enableDebugPhysicsDraw;
    protected:
        void updateVisibleRect();
        void tick(float dt);
        void initPhysics();
        void initLandscapePhysics();
        void initDynamicObjectsPhysics();
    public:
        virtual ~GameScreen();
        virtual bool init() override;
        void loadMap(size_t mapIndex);
        
        Map* getMap() const { return map; }
        Character* getCharacter() const { return character; }
        Longboard* getLongboard() const { return longboard; }
        b2World* getWorld() const { return world; }
        GameHUDLayer* getHUDLayer() const { return hudLayer; }
        
        void onPushButtonTouchBegin();
        void onPushButtonTouchEnd();
        void onChangeEnableDebugDrawPhysics();
        
        CREATE_FUNC(GameScreen);
    };
}

#endif /* GameScreen_hpp */
