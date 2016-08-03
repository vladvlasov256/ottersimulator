//
//  Character.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 02.05.16.
//
//

#ifndef Character_hpp
#define Character_hpp

#include "cocos2d.h"
#include "COCharacterRenderer.h"
#include "Longboard.hpp"
#include "external/Box2D/Box2D.h"//!!!headers path

namespace ottersimulator
{
    class Character : public cocos2d::Node
    {
    protected:
        enum PushState
        {
            kIdle = 0,
            kGoingDown = 1,
            kOnTheGround = 2,
            kBraking = 3,
            kGoingUp = 4
        };
    private:
        COCharacterRenderer* characterRenderer;
        std::map<std::string, b2Body*> boneBodies;
        b2PrismaticJoint* longboardJointBack;
        b2PrismaticJoint* longboardJointFront;
        b2PrismaticJoint* legJoint;
        b2RevoluteJoint* torsoJoint;
        std::map<std::string, cocos2d::Vec2> initialPositions;
        std::map<std::string, float> initialRotations;
        cocos2d::DrawNode* debugDrawNode;//!!!debug
        PushState pushState;
        cocos2d::Vec2 footPushShift;
        float footShiftDistance;
        std::chrono::time_point<std::chrono::system_clock> pushLastStateBegin;
    protected:
        static float getBoneLength(spBone* bone, spBone* parentBone);
        void updateLegTransforms(const std::string& boneNamePrefix);
        void updatePushState();
        cocos2d::Vec2 getFootPosition(const std::string& boneNamePrefix, bool withPushShift=true);
        static std::chrono::time_point<std::chrono::system_clock> getCurrentTime();
        void updateFootShiftPosition();
    public:
        virtual bool initWithSkeletonAndAtlas(const std::string& skeletonFileName, const std::string& atlasFileName);
        
        static Character* createWithSkeletonAndAtlas(const std::string& skeletonFileName, const std::string& atlasFileName);
        
        void initPhysics();
        void updateSprite(b2Body* body);
        void updateLegTransforms();
        void onTick();
        
        void onTouchMoved(cocos2d::Touch* touch);
        void putOnLongboard(Longboard* longboard);
        
        COCharacterRenderer* getCharacterRenderer() { return characterRenderer; }
        cocos2d::Vec2 getBonePosition(spBone* bone, spBone* rootBone) const;
        cocos2d::Vec2 getInitialBonePosition(const std::string& boneName) { return initialPositions[boneName]; }
        float getInitialBoneRotation(const std::string& boneName) { return initialRotations[boneName]; }
        
        void startPush();
        void endPush();
    };
}

#endif /* Character_hpp */
