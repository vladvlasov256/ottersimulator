//
//  Longboard.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 02.05.16.
//
//

#ifndef Longboard_hpp
#define Longboard_hpp

#include "cocos2d.h"
#include "Box2D/Box2D.h"

namespace ottersimulator
{
    class Longboard : public cocos2d::Node
    {
    private:
        cocos2d::Sprite* board;
        cocos2d::Sprite* backWheel;
        cocos2d::Sprite* frontWheel;
        b2Body* boardBody;
        b2Body* backWheelBody;
        b2Body* frontWheelBody;
        b2RevoluteJoint* backJoint;
        b2RevoluteJoint* frontJoint;
    protected:
        void blockWheelJoint(b2RevoluteJoint* joint);
    public:
        virtual bool initWithTexureAndWheel(const std::string& textureFileName, const std::string& wheelFileName);
        
        static Longboard* createWithTexureAndWheel(const std::string& textureFileName, const std::string& wheelFileName);
        
        void initPhysics();
        void updateSprite(b2Body* body);
        
        void applyPush();
        void applyBrake(float elapsedTime);
        void endBrake();
        
        b2Body* getBoardBody() const { return boardBody; }
    };
}

#endif /* Longboard_hpp */
