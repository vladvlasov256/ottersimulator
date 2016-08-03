//
//  Character.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 02.05.16.
//
//

#include "global.h"
#include "Character.hpp"
#include "GameScreen.hpp"

USING_NS_CC;
using namespace ottersimulator;

#define SCALE .65f
#define CENTER Vec2(0.f, .15f)

const std::string LEFT_FOOT = "left foot";
const std::string LEFT_LOWER_LEG = "left lower leg";
const std::string LEFT_UPPER_LEG = "left upper leg";
const std::string RIGHT_FOOT = "right foot";
const std::string RIGHT_LOWER_LEG = "right lower leg";
const std::string RIGHT_UPPER_LEG = "right upper leg";
const std::string HIP = "hip";
const std::string TORSO = "torso";
const std::string ROOT = "root";
const std::string DUMMY_HIP = "dummy hip";
const std::string DUMMY_TORSO = "dummy torso";

const float PUSH_GOING_DOWN_DURATION = 300.f;
const float PUSH_ON_THE_GROUND_DURATION = 300.f;
const float PUSH_GOING_UP_DURATION = 300.f;

class LegInverseKinematics
{
private:
    Character* character;
    std::string boneNamePrefix;
    Vec2 footPosition;
    float upperLegRotation;
    float lowerLegRotation;
    float footRotation;
    Vec2 upperLegPosition;
    float alpha3, alpha4, beta1, beta2;
    float upperLegLength;
    Vec2 lowerLegShift;
    Vec2 footShift;
    float lowerLegLength;
    float upperLegLengthWithShift;
    float lowerLegLengthWithShift;
protected:
    void getTriangleAngles(float l0, float l1, float l2, float& angle0, float& angle1, float& angle2) const;
    float getTriangleAngle(float l0, float l1, float l2) const;
public:
    LegInverseKinematics(Character* character, const std::string& boneNamePrefix, const Vec2& footPosition);
    float getUpperLegRotation() const { return upperLegRotation; }
    float getLowerLegRotation() const { return lowerLegRotation; }
    float getFootRotation() const { return footRotation; }
    void addDebugDrawing(DrawNode* drawNode, const Vec2& globalPosition, float scale=1.f) const;
};

bool Character::initWithSkeletonAndAtlas(const std::string& skeletonFileName, const std::string& atlasFileName)
{
    if(!Node::init())
    {
        return false;
    }
    
    this->characterRenderer = COCharacterRenderer::createWithFile(skeletonFileName, atlasFileName);
    this->characterRenderer->setScale(UNITS_IN_SCREEN_HEIGHT / Director::getInstance()->getWinSize().height * SCALE);
    this->addChild(characterRenderer);
    
    this->debugDrawNode = DrawNode::create();
    this->addChild(this->debugDrawNode);
    
    return true;
}

Character* Character::createWithSkeletonAndAtlas(const std::string& skeletonFileName, const std::string& atlasFileName)
{
    Character *character = new (std::nothrow) Character();
    if (character && character->initWithSkeletonAndAtlas(skeletonFileName, atlasFileName))
    {
        character->autorelease();
        return character;
    }
    CC_SAFE_DELETE(character);
    return nullptr;
}

void Character::initPhysics()
{
    this->boneBodies.clear();
    
    spSkeleton_updateWorldTransform(this->characterRenderer->getSkeleton());
    
    auto rootBone = this->characterRenderer->findBone(ROOT);
    for (int i = 0, n = this->characterRenderer->getSkeleton()->bonesCount; i < n; i++)
    {
        spBone *bone = this->characterRenderer->getSkeleton()->bones[i];
        this->initialPositions[bone->data->name] = this->getBonePosition(bone, rootBone) - this->getPosition();//!!!method
        this->initialRotations[bone->data->name] = bone->rotation;
    }
    
    auto world = static_cast<GameScreen*>(getScene())->getWorld();
    
//    b2BodyDef bodyDef;
//    bodyDef.type = b2_dynamicBody;
//    bodyDef.position.Set(this->getPosition().x + CENTER.x, this->getPosition().y + CENTER.y);
//    bodyDef.userData = this;
//    this->body = world->CreateBody(&bodyDef);
//    
//    b2PolygonShape bodyShape;
    //    bodyShape.SetAsBox(.2f, CENTER.y);
    
    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(.1f, .05f);
    
    b2FixtureDef bodyhapeDef;
    bodyhapeDef.shape = &bodyShape;
    bodyhapeDef.density = 50.f;
    bodyhapeDef.friction = 20.6f;
    bodyhapeDef.restitution = 0.f;//.01f;
    
    //Right foot
    auto rightFootBone = this->characterRenderer->findBone(RIGHT_FOOT);
//    this->initialPositions[RIGHT_FOOT] = getBonePosition(rightFootBone, rootBone) - this->getPosition();//!!!method
//    this->initialRotations[RIGHT_FOOT] = rightFootBone->rotation;
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->getPosition().x - .2f, this->getPosition().y + .05f);
    bodyDef.userData = (void*)&RIGHT_FOOT;
    
    b2Body* rightFootBody = world->CreateBody(&bodyDef);
    rightFootBody->CreateFixture(&bodyhapeDef);
    this->boneBodies[RIGHT_FOOT] = rightFootBody;
    
    //Left foot
    auto leftFootBone = this->characterRenderer->findBone(LEFT_FOOT);
//    this->initialPositions[LEFT_FOOT] = getBonePosition(leftFootBone, rootBone) - this->getPosition();
//    this->initialRotations[LEFT_FOOT] = leftFootBone->rotation;
    
    bodyDef.position.Set(this->getPosition().x + .2f, this->getPosition().y + .05f);
    bodyDef.userData = (void*)&LEFT_FOOT;
    
    b2Body* leftFootBody = world->CreateBody(&bodyDef);
    leftFootBody->CreateFixture(&bodyhapeDef);
    this->boneBodies[LEFT_FOOT] = leftFootBody;
    
    //Dummy hip
    auto hipBone = this->characterRenderer->findBone(HIP);
    Vec2 hipBonePosition = getBonePosition(hipBone, rootBone);
//    this->initialPositions[HIP] = hipBonePosition - this->getPosition();
//    this->initialRotations[HIP] = hipBone->rotation;
    
    bodyhapeDef.density = 500.f;
    bodyShape.SetAsBox(.025f, .025f);
    
    bodyDef.position.Set(hipBonePosition.x, hipBonePosition.y);
    bodyDef.userData = (void*)&DUMMY_HIP;
    
    b2Body* dummyHipBody = world->CreateBody(&bodyDef);
    dummyHipBody->CreateFixture(&bodyhapeDef);
    this->boneBodies[DUMMY_HIP] = dummyHipBody;
    
    //Dummy torso
    auto torsoBone = this->characterRenderer->findBone(TORSO);
    Vec2 torsoBonePosition = getBonePosition(torsoBone, rootBone);
//    this->initialPositions[TORSO] = torsoBonePosition - this->getPosition();
//    this->initialRotations[TORSO] = torsoBone->rotation;
    
    bodyhapeDef.density = 1.f;
    bodyShape.SetAsBox(.125f, .25f);
    
    bodyDef.position.Set(hipBonePosition.x, hipBonePosition.y + .25f);
    bodyDef.userData = (void*)&DUMMY_TORSO;
    
    b2Body* dummyTorsoBody = world->CreateBody(&bodyDef);
    dummyTorsoBody->CreateFixture(&bodyhapeDef);
    this->boneBodies[DUMMY_TORSO] = dummyTorsoBody;

    b2RevoluteJointDef torsoJointDef;
    torsoJointDef.bodyA = dummyTorsoBody;
    torsoJointDef.bodyB = dummyHipBody;
    torsoJointDef.localAnchorA = b2Vec2(0.f, -.25f);
//    torsoJointDef.localAnchorA = b2Vec2(0.f, 0.f);
    torsoJointDef.collideConnected = false;
    torsoJointDef.enableLimit = true;
    torsoJointDef.lowerAngle = 0.f * M_PI - .05f;
    torsoJointDef.upperAngle = 0.f * M_PI + .05f;
    
    this->torsoJoint = (b2RevoluteJoint*)world->CreateJoint(&torsoJointDef);
}

float Character::getBoneLength(spBone* bone, spBone* parentBone)
{
    Vec2 boneVec = Vec2(bone->worldX - parentBone->worldX, bone->worldY - parentBone->worldY);
    return boneVec.getLength() * UNITS_IN_SCREEN_HEIGHT / Director::getInstance()->getWinSize().height * SCALE;
}

Vec2 Character::getBonePosition(spBone* bone, spBone* rootBone) const
{
    Vec2 boneVec = Vec2(bone->worldX - rootBone->worldX, bone->worldY - rootBone->worldY);
    return boneVec * UNITS_IN_SCREEN_HEIGHT / Director::getInstance()->getWinSize().height * SCALE + this->getPosition();
}

//!!!return TRUE, if a bone has been processed
void Character::updateSprite(b2Body* body)
{
    if(body->GetUserData() == &DUMMY_TORSO)
    {
        auto torsoBone = this->characterRenderer->findBone(TORSO);
        torsoBone->rotation = CC_RADIANS_TO_DEGREES(body->GetAngle()) + this->initialRotations[TORSO];
    }
    else if(body->GetUserData() == &DUMMY_HIP)
    {
        //        float rotation = -CC_RADIANS_TO_DEGREES(body->GetAngle());
        //        Vec2 anchorShift = this->torsoInitialPosition;
        //        anchorShift.rotate(Vec2(), body->GetAngle());
        
        this->setPosition(Vec2(body->GetPosition().x, body->GetPosition().y) - this->initialPositions[HIP]);// - CENTER);
        
        auto hipBone = this->characterRenderer->findBone(HIP);
        hipBone->rotation = CC_RADIANS_TO_DEGREES(body->GetAngle()) + this->initialRotations[HIP];
    }
    else if(body->GetUserData() == &LEFT_FOOT)
    {
//        auto leftFootBone = this->characterRenderer->findBone(LEFT_FOOT);
//        leftFootBone->rotation = CC_RADIANS_TO_DEGREES(body->GetAngle()) + this->initialRotations[LEFT_FOOT];//!!!remove
    }
    else if(body->GetUserData() == &RIGHT_FOOT)
    {
//        auto rightFootBone = this->characterRenderer->findBone(RIGHT_FOOT);
//        rightFootBone->rotation = CC_RADIANS_TO_DEGREES(body->GetAngle()) + this->initialRotations[RIGHT_FOOT];
    }
}

void Character::updateLegTransforms()
{
    this->debugDrawNode->clear();//!!!debug
    this->updateLegTransforms("left ");
    this->updateLegTransforms("right ");
}

void Character::updateLegTransforms(const std::string& boneNamePrefix)
{
    Vec2 footPosition = this->getFootPosition(boneNamePrefix);
    LegInverseKinematics legInverseKinematics(this, boneNamePrefix, footPosition);
    
    auto upperLegBone = this->characterRenderer->findBone(boneNamePrefix + "upper leg");
    upperLegBone->rotation = legInverseKinematics.getUpperLegRotation();
    
    auto lowerLegBone = this->characterRenderer->findBone(boneNamePrefix + "lower leg");
    lowerLegBone->rotation = legInverseKinematics.getLowerLegRotation();
    
    auto footBone = this->characterRenderer->findBone(boneNamePrefix + "foot");
    footBone->rotation = legInverseKinematics.getFootRotation();
    
    //!!!debug
//    legInverseKinematics.addDebugDrawing(this->debugDrawNode, this->getPosition(), 3);
}

Vec2 Character::getFootPosition(const std::string& boneNamePrefix, bool withPushShift)
{
    if(boneNamePrefix.find(std::string("left")) != std::string::npos || (this->pushState == kIdle) || !withPushShift)
    {
        auto hipBone = this->characterRenderer->findBone(HIP);
        float hipRotationInRadians = CC_DEGREES_TO_RADIANS(hipBone->rotation - this->initialRotations[HIP]);
        
        Vec2 footBodyShift(-.05f, -.02f);
        footBodyShift.rotate(Vec2::ZERO, hipRotationInRadians);
        
        auto footBody = this->boneBodies[boneNamePrefix + "foot"];
        return Vec2(footBody->GetPosition().x, footBody->GetPosition().y) + footBodyShift;
    }
    else
    {
        return this->getFootPosition(boneNamePrefix, false) + this->footPushShift;
    }
}

void Character::onTick()
{
    this->updatePushState();
    this->updateLegTransforms();
}

void Character::onTouchMoved(Touch* touch)
{
//    this->characterRender->onTouchMoved(touch);
    
//    Vec2 force = Vec2(touch->getDelta().x, touch->getDelta().y) * .1f;
//    b2Body* hipBody = this->boneBodies[HIP];
//    hipBody->ApplyLinearImpulse(b2Vec2(force.x, force.y), b2Vec2(hipBody->GetPosition().x, hipBody->GetPosition().y), true);
    
    float distance = MAX(MIN(this->legJoint->GetLowerLimit() + touch->getDelta().y * .002f, .34f), .25f);
//    float distance = MAX(MIN(this->legJoint->GetLowerLimit() + touch->getDelta().y * .002f, .42f), .25f);
    this->legJoint->SetLimits(distance, distance + .1f);
    
    float rotation = CC_DEGREES_TO_RADIANS(MAX(MIN(CC_RADIANS_TO_DEGREES(this->torsoJoint->GetLowerLimit()) + touch->getDelta().x * .3f, 90.f), -10.f));
    this->torsoJoint->SetLimits(rotation, rotation);
}

std::chrono::time_point<std::chrono::system_clock> Character::getCurrentTime()
{
//    time_t _time = time(0);
//    return *std::localtime(&_time);
    return std::chrono::system_clock::now();
}

void Character::startPush()
{
    if(this->pushState == PushState::kIdle || this->pushState == PushState::kGoingUp)
    {
        this->pushState = PushState::kGoingDown;
        this->pushLastStateBegin = Character::getCurrentTime();
        auto gameScreen = static_cast<GameScreen*>(getScene());
        gameScreen->getHUDLayer()->setPushIconVisible(true);
    }
}

void Character::endPush()
{
    auto gameScreen = static_cast<GameScreen*>(getScene());
    
    switch (this->pushState)
    {
        case PushState::kGoingDown:
            this->pushLastStateBegin = Character::getCurrentTime();
            this->pushState = PushState::kGoingUp;
            gameScreen->getHUDLayer()->setPushIconVisible(false);
            break;
        case PushState::kOnTheGround:
            this->pushLastStateBegin = Character::getCurrentTime();
            this->pushState = PushState::kGoingUp;
            gameScreen->getHUDLayer()->setPushIconVisible(false);
            gameScreen->getLongboard()->applyPush();
            break;
        case PushState::kBraking:
            this->pushLastStateBegin = Character::getCurrentTime();
            this->pushState = PushState::kGoingUp;
            gameScreen->getHUDLayer()->setPushIconVisible(false);
            gameScreen->getLongboard()->endBrake();
            break;
            
        default:
            break;
    }
}

void Character::updatePushState()
{
    auto currentTime = this->getCurrentTime();
    static auto lastUpdatePushTime = currentTime;
    double stateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->pushLastStateBegin).count();
    double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdatePushTime).count();
    lastUpdatePushTime = currentTime;
    float maxShift = -.15f;//!!!param;
    auto gameScreen = static_cast<GameScreen*>(getScene());
    
    switch (this->pushState)
    {
        case PushState::kGoingDown:
            if(this->footPushShift.y < maxShift)
            {
                this->footShiftDistance = maxShift;
                this->pushState = PushState::kOnTheGround;
                this->pushLastStateBegin = Character::getCurrentTime();
                gameScreen->getHUDLayer()->setPushIconProgress(100.f);
            }
            else
            {
                this->footShiftDistance += maxShift * elapsedTime / PUSH_GOING_DOWN_DURATION;
            }
            break;
        case PushState::kOnTheGround:
            if(stateDuration > PUSH_ON_THE_GROUND_DURATION)
            {
                this->pushState = PushState::kBraking;
                this->pushLastStateBegin = Character::getCurrentTime();
                gameScreen->getHUDLayer()->setPushIconProgress(0.f);
            }
            else
            {
                gameScreen->getHUDLayer()->setPushIconProgress(100.f * (1.f - stateDuration / PUSH_ON_THE_GROUND_DURATION));
            }
            break;
        case PushState::kBraking:
            {
                gameScreen->getLongboard()->applyBrake(elapsedTime * .001f);
            }
            break;
        case PushState::kGoingUp:
            if(this->footPushShift.y > 0)
            {
                this->footShiftDistance = 0.f;
                this->pushState = PushState::kIdle;
                this->pushLastStateBegin = Character::getCurrentTime();
            }
            else
            {
                this->footShiftDistance -= maxShift * elapsedTime / PUSH_GOING_UP_DURATION;
            }
            break;
            
        default:
            break;
    }
    
    updateFootShiftPosition();
}

void Character::updateFootShiftPosition()
{
    this->footPushShift = Vec2(0, footShiftDistance);
    this->footPushShift.rotate(Vec2::ZERO, CC_DEGREES_TO_RADIANS(this->getRotation()));
}

void Character::putOnLongboard(Longboard* longboard)
{
    //Left foot
    b2PrismaticJointDef prismaticJointDef;
    prismaticJointDef.bodyA = this->boneBodies[RIGHT_FOOT];
    prismaticJointDef.bodyB = longboard->getBoardBody();
    prismaticJointDef.collideConnected = false;
    prismaticJointDef.localAxisA.Set(0, -1);
    prismaticJointDef.localAnchorA.Set(0.f, -.05f);
    prismaticJointDef.localAnchorB.Set(-.15f, .01f);
    prismaticJointDef.enableLimit = true;
    prismaticJointDef.lowerTranslation = 0;
    prismaticJointDef.upperTranslation = 0;
    
    auto world = static_cast<GameScreen*>(getScene())->getWorld();
    this->longboardJointBack = (b2PrismaticJoint*)world->CreateJoint(&prismaticJointDef);
    
    //Right foot
    prismaticJointDef.bodyA = this->boneBodies[LEFT_FOOT];
    prismaticJointDef.localAnchorA.Set(0.f, -.05f);
    prismaticJointDef.localAnchorB.Set(.1f, .01f);
    this->longboardJointFront = (b2PrismaticJoint*)world->CreateJoint(&prismaticJointDef);
    
    //Dummy hip
    prismaticJointDef.bodyA = this->boneBodies[DUMMY_HIP];
    prismaticJointDef.localAnchorA.Set(0.f, 0.f);
    prismaticJointDef.localAnchorB.Set(0.f, .01f);
    prismaticJointDef.lowerTranslation = .35f;
    prismaticJointDef.upperTranslation = .36f;
    this->legJoint = (b2PrismaticJoint*)world->CreateJoint(&prismaticJointDef);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LegInverseKinematics::LegInverseKinematics(Character* character, const std::string& prefix, const Vec2& footPos) :
    character(character), boneNamePrefix(prefix), footPosition(footPos)
{
    auto characterRenderer = character->getCharacterRenderer();
    
    auto rootBone = characterRenderer->findBone(ROOT);
    auto hipBone = characterRenderer->findBone(HIP);
    auto upperLegBone = characterRenderer->findBone(boneNamePrefix + "upper leg");
    auto lowerLegBone = characterRenderer->findBone(boneNamePrefix + "lower leg");
    
    Vec2 upperLegInitialPos = this->character->getInitialBonePosition(boneNamePrefix + "upper leg");
    Vec2 lowerLegInitialPos =  this->character->getInitialBonePosition(boneNamePrefix + "lower leg");
    
    float upperLegInitialRotation = this->character->getInitialBoneRotation(boneNamePrefix + "upper leg");
    float upperLegInitialRotationInRadians = CC_DEGREES_TO_RADIANS(upperLegInitialRotation);
    float lowerLegInitialRotation = this->character->getInitialBoneRotation(boneNamePrefix + "lower leg");
    float lowerLegTotalInitialRotationInRadians = CC_DEGREES_TO_RADIANS(upperLegInitialRotation + lowerLegInitialRotation);
    
    float boneScale = UNITS_IN_SCREEN_HEIGHT / Director::getInstance()->getWinSize().height * SCALE;
    this->upperLegLength = upperLegBone->data->length * boneScale;
    this->lowerLegLength = lowerLegBone->data->length * boneScale;
    
    float hipInitialRotation = this->character->getInitialBoneRotation(HIP);
    float hipRotation = hipBone->rotation;
    float hipRotationInRadians = CC_DEGREES_TO_RADIANS(hipRotation - hipInitialRotation);
    
    Vec2 upperLegShift = upperLegInitialPos - this->character->getInitialBonePosition(HIP);
    upperLegShift.rotate(Vec2::ZERO, hipRotationInRadians);
    this->upperLegPosition = this->character->getBonePosition(hipBone, rootBone) + upperLegShift;
    
    this->upperLegLengthWithShift = (upperLegInitialPos - lowerLegInitialPos).getLength();
    
    Vec2 footInitialPos =  this->character->getInitialBonePosition(boneNamePrefix + "foot");
    this->lowerLegLengthWithShift = (footInitialPos - lowerLegInitialPos).getLength();
    
    float legLength = (footPosition - this->upperLegPosition).getLength();
    
    float alpha, beta, gamma;
    this->getTriangleAngles(this->upperLegLengthWithShift, this->lowerLegLengthWithShift, legLength, alpha, beta, gamma);
    
    float legHeight = abs(footPosition.y - this->upperLegPosition.y);
    float alpha1 = acos(legHeight / legLength);
    
    Vec2 upperLegVec(upperLegLength, 0);
    upperLegVec.rotate(Vec2::ZERO, upperLegInitialRotationInRadians);
    this->lowerLegShift = lowerLegInitialPos - (upperLegInitialPos + upperLegVec);
    this->lowerLegShift.rotate(Vec2::ZERO, upperLegInitialRotationInRadians);
    
    this->alpha4 = this->getTriangleAngle(this->lowerLegShift.getLength(), this->upperLegLengthWithShift, this->upperLegLength);
    this->alpha3 = (this->footPosition.x < this->upperLegPosition.x) ? (M_PI_2 + alpha1 - alpha + this->alpha4) : (M_PI_2 - alpha1 - alpha + this->alpha4);
    
    this->upperLegRotation = CC_RADIANS_TO_DEGREES(-this->alpha3 - hipRotationInRadians);
    
    Vec2 lowerLegVec(this->lowerLegLength, 0);
    lowerLegVec.rotate(Vec2::ZERO, lowerLegTotalInitialRotationInRadians);
    this->footShift = footInitialPos - (lowerLegInitialPos + lowerLegVec);
//    this->footShift.rotate(Vec2::ZERO, lowerLegTotalInitialRotationInRadians);
    
    this->beta2 = (this->footShift.getLength() > .001f) ? this->getTriangleAngle(this->footShift.getLength(), this->lowerLegLengthWithShift, this->lowerLegLength) : 0.f;
    
    this->beta1 = -M_PI - this->alpha3 + this->alpha4 + beta + this->beta2;
    
    this->lowerLegRotation = CC_RADIANS_TO_DEGREES(this->beta1 + this->alpha3);
    
    this->footRotation = this->character->getInitialBoneRotation(boneNamePrefix + "foot") - upperLegBone->rotation - lowerLegBone->rotation + hipInitialRotation + upperLegInitialRotation + lowerLegInitialRotation;
}

void LegInverseKinematics::addDebugDrawing(DrawNode* drawNode, const Vec2& globalPosition, float scale) const
{
    //TODO: fix
    drawNode->setLineWidth(1);
    
    Vec2 upperLegVec(this->upperLegLength, 0);
    upperLegVec.rotate(Vec2::ZERO, -this->alpha3);
    Vec2 p3 = this->upperLegPosition + upperLegVec;
    drawNode->drawLine(this->upperLegPosition - globalPosition, p3 - globalPosition, Color4F(1.f, .5f, 1.f, 1.f));
    
    Vec2 lowerLegShiftVec(this->lowerLegShift);
    float upperLegInitialRotation = this->character->getInitialBoneRotation(boneNamePrefix + "upper leg");
    float upperLegInitialRotationInRadians = CC_DEGREES_TO_RADIANS(upperLegInitialRotation);
    lowerLegShiftVec.rotate(Vec2::ZERO, -this->alpha3 - M_PI);// - upperLegInitialRotationInRadians);
    Vec2 _p1 = p3 + lowerLegShiftVec;
    drawNode->drawLine(_p1 - globalPosition, p3 - globalPosition, Color4F(0, 0, 0, 1.f));
    
    Vec2 lowerLegVec(this->lowerLegLength, 0);
    lowerLegVec.rotate(Vec2::ZERO, this->beta1);
    Vec2 p4 = _p1 + lowerLegVec;
    drawNode->drawLine(_p1 - globalPosition, p4 - globalPosition, Color4F(.5f, 1.f, 1.f, 1.f));
    
    Vec2 footShiftVec = this->footShift;
    float lowerLegInitialRotation = this->character->getInitialBoneRotation(boneNamePrefix + "lower leg");
    float lowerLegTotalInitialRotationInRadians = CC_DEGREES_TO_RADIANS(upperLegInitialRotation + lowerLegInitialRotation);
    footShiftVec.rotate(Vec2::ZERO, this->beta1 - upperLegInitialRotationInRadians);
    Vec2 p5 = p4 + footShiftVec;
    drawNode->drawLine(p4 - globalPosition, p5 - globalPosition, Color4F(1.f, 0, 0, 1.f));
    
    drawNode->drawLine(this->upperLegPosition - globalPosition, footPosition - globalPosition, Color4F(1.f, 1.f, .5f, 1.f));
    
    drawNode->setScale(scale);
    
//    Vec2 upperLegLengthWithShiftVec(this->upperLegLengthWithShift, 0);
//    upperLegLengthWithShiftVec.rotate(Vec2::ZERO, -this->alpha3 + this->alpha4);
//    drawNode->drawLine(this->upperLegPosition - globalPosition, this->upperLegPosition + upperLegLengthWithShiftVec - globalPosition, Color4F(0, 1.f, 0, .3f));
//    
//    Vec2 lowerLegLengthWithShiftVec(this->lowerLegLengthWithShift, 0);
//    lowerLegLengthWithShiftVec.rotate(Vec2::ZERO, this->beta1 - this->beta2);
//    drawNode->drawLine(this->upperLegPosition + upperLegLengthWithShiftVec - globalPosition, this->upperLegPosition + upperLegLengthWithShiftVec + lowerLegLengthWithShiftVec - globalPosition, Color4F(0, 1.f, 0, .3f));
}

void LegInverseKinematics::getTriangleAngles(float l0, float l1, float l2, float& angle0, float& angle1, float& angle2) const
{
    angle0 = this->getTriangleAngle(l1, l0, l2);
    angle2 = this->getTriangleAngle(l0, l1, l2);
    angle1 = M_PI - angle0 - angle2;
}

float LegInverseKinematics::getTriangleAngle(float l0, float l1, float l2) const
{
    float l0_2 = l0 * l0;
    float l1_2 = l1 * l1;
    float l2_2 = l2 * l2;
    return acos((l1_2 + l2_2 - l0_2) / (2 * l1 * l2));
}

//void Character::updateLegTransforms(const std::string& boneNamePrefix)
//{
//    auto rootBone = this->characterRender->findBone(ROOT);
//    auto hipBone = this->characterRender->findBone(HIP);
//    auto upperLegBone = this->characterRender->findBone(boneNamePrefix + "upper leg");
//    auto lowerLegBone = this->characterRender->findBone(boneNamePrefix + "lower leg");
//
//    Vec2 upperLegInitialPos = this->initialPositions[boneNamePrefix + "upper leg"];
//    Vec2 lowerLegInitialPos = this->initialPositions[boneNamePrefix + "lower leg"];
//
//    float upperLegInitialRotation = this->initialRotations[boneNamePrefix + "upper leg"];
//    float upperLegInitialRotationInRadians = CC_DEGREES_TO_RADIANS(upperLegInitialRotation);
//    float lowerLegInitialRotation = this->initialRotations[boneNamePrefix + "lower leg"];
//    float lowerLegTotalInitialRotationInRadians = CC_DEGREES_TO_RADIANS(upperLegInitialRotation + lowerLegInitialRotation);
//
//    auto footBody = this->boneBodies[boneNamePrefix + "foot"];
//
//    float upperLegLength = upperLegBone->data->length * UNITS_IN_SCREEN_HEIGHT / Director::getInstance()->getWinSize().height * SCALE;
//    float lowerLegLength = lowerLegBone->data->length * UNITS_IN_SCREEN_HEIGHT / Director::getInstance()->getWinSize().height * SCALE;
//
//    float hipInitialRotation = this->initialRotations[HIP];
//    float hipRotation = hipBone->rotation;
//    float hipRotationInRadians = CC_DEGREES_TO_RADIANS(hipRotation);
//
//    Vec2 upperLegShift = upperLegInitialPos - this->initialPositions[HIP];
//    upperLegShift.rotate(Vec2::ZERO, hipRotationInRadians);
//    Vec2 p0 = this->getBonePosition(hipBone, rootBone) + upperLegShift;
//    //    printf("p0 = (%.3f, %.3f)\n", p0.x, p0.y);//!!delete
//
//    float l0 = (upperLegInitialPos - lowerLegInitialPos).getLength();
//
//    Vec2 footInitialPos = this->initialPositions[boneNamePrefix + "foot"];
//    float l1 = (footInitialPos - lowerLegInitialPos).getLength();
//
//    Vec2 lowerLegVec(lowerLegLength, 0);
//    lowerLegVec.rotate(Vec2::ZERO, lowerLegTotalInitialRotationInRadians);
//    Vec2 footShift = footInitialPos - (lowerLegInitialPos + lowerLegVec);
//    footShift.rotate(Vec2::ZERO, lowerLegTotalInitialRotationInRadians);
//    printf("footShift = (%.3f, %.3f)\n", footShift.x, footShift.y);//!!delete
//
//    Vec2 footBodyShift(-.05f, -.02f);
//    footBodyShift.rotate(Vec2::ZERO, hipRotationInRadians);
//    Vec2 p2 = Vec2(footBody->GetPosition().x, footBody->GetPosition().y) + footBodyShift;
//    float l2 = (p2 - p0).getLength();
//    //    printf("l2 = %.3f\n", l2);//!!delete
//
//    float alpha = acos((l0 * l0 + l2 * l2 - l1 * l1) / (2 * l0 * l2));
//    float gamma = acos((l1 * l1 + l2 * l2 - l0 * l0) / (2 * l1 * l2));
//    float beta = M_PI - alpha - gamma;
//
//    float h = abs(p2.y - p0.y);
//    float alpha1 = acos(h / l2);
//
//    //    //!!!2 methods
//    //    float globalRotation = CC_DEGREES_TO_RADIANS(hipRotation - hipInitialRotation);
//    //    Vec2 longboardVector(1, 0);
//    //    longboardVector.rotate(Vec2::ZERO, globalRotation);
//    //    Vec2 longboardPoint = p2 + longboardVector;
//    //    float dx = p2.x - longboardPoint.x;
//    //    float dy = p2.y - longboardPoint.y;
//    //    float longboardA, longboardC;
//    //    float longboardB = 1.f;
//    //    if(abs(dy) < 0.001)
//    //    {
//    //        longboardA = 0;
//    //        longboardC = -p2.y;
//    //    }
//    //    else
//    //    {
//    //        float slope = dy / dx;
//    //        longboardC = -longboardPoint.y + slope * longboardPoint.x;
//    //        longboardA = -slope;
//    //    }
//    //    float h = (longboardA * p0.x + longboardB * p0.y + longboardC) / sqrt(longboardA * longboardA + longboardB * longboardB);//!!!method
//    //    float alpha1 = acos(h / l2);
//
//    Vec2 upperLegVec(upperLegLength, 0);
//    upperLegVec.rotate(Vec2(), upperLegInitialRotationInRadians);
//    Vec2 lowerLegShift = lowerLegInitialPos - (upperLegInitialPos + upperLegVec);
//    lowerLegShift.rotate(Vec2::ZERO, upperLegInitialRotationInRadians);
//    //    printf("lowerLegShift = (%.3f, %.3f)\n", lowerLegShift.x, lowerLegShift.y);//!!delete
//    float m = lowerLegShift.getLength();
//    float alpha4 = acos((l0 * l0 + upperLegLength * upperLegLength - m * m) / (2 * l0 * upperLegLength));
//
//    //    Vec2 rayToLongboard(0.f, -10.f);
//    //    rayToLongboard.rotate(Vec2::ZERO, globalRotation); ///!!!!???
//    //    rayToLongboard += p0;
//    //    bool isP2LeftFromP0 = (p2.y - p0.y) * (rayToLongboard.x - p0.x) > (p2.x - p0.x) * (rayToLongboard.y - p0.y);
//    //    float alpha3 = !isP2LeftFromP0 ? (M_PI_2 + alpha1 - alpha + alpha4) : (M_PI_2 - alpha1 - alpha + alpha4);
//
//    float alpha3 = (p2.x < p0.x) ? (M_PI_2 + alpha1 - alpha + alpha4) : (M_PI_2 - alpha1 - alpha + alpha4);
//
//
//
//    float n = footShift.getLength();
//    float beta2 = (n > .001f) ? acos((l1 * l1 + lowerLegLength * lowerLegLength - n * n) / (2 * l1 * lowerLegLength)) : 0.f;
//
//    Vec2 footShiftRotated = -footShift;
//    footShiftRotated.rotate(Vec2::ZERO, -(-M_PI + beta - alpha3));
//    //    float beta1 = (footShiftRotated.y < 0) ? (beta - beta2) : (beta + beta2); //???
//    float beta1 = -M_PI - alpha3 + alpha4 + beta + beta2;
//
//    //    printf("alpha = %.3f + %.3f, beta = %.3f, gamma = %.3f\n", alpha1, alpha2, beta, gamma);//!!!debug
//
//    upperLegBone->rotation = CC_RADIANS_TO_DEGREES(-alpha3 - hipRotationInRadians);// + this->initialRotations[boneNamePrefix + "upper leg"];
//
//    lowerLegBone->rotation = CC_RADIANS_TO_DEGREES(beta1 + alpha3); //- CC_RADIANS_TO_DEGREES(M_PI_2);// + this->initialRotations[boneNamePrefix + "lower leg"];
//
//    auto footBone = this->characterRender->findBone(boneNamePrefix + "foot");
//    footBone->rotation = footBone->rotation - hipRotation - upperLegBone->rotation - lowerLegBone->rotation + hipInitialRotation + upperLegInitialRotation + lowerLegInitialRotation;// + this->initialRotations[boneNamePrefix + "foot"];
//
//    //!!!debug
//    //    Vec2 tempLL(l1, 0);
//    //    tempLL.rotate(Vec2(0, 0),  CC_DEGREES_TO_RADIANS(this->initialRotations[boneNamePrefix + "upper leg"] + this->initialRotations[boneNamePrefix + "lower leg"]));
//    //    Vec2 tempP2 = lowerLegInitialPos + tempLL;
//    //    this->debugDrawNode->drawLine(lowerLegInitialPos, tempP2, Color4F(.5f, 1.f, 1.f, 1.f));
//    //    this->debugDrawNode->drawLine(tempP2, footInitialPos, Color4F(1.f, .5f, 1.f, 1.f));
//
//    //!!!debug
//    //    Vec2 tempLU(l3, 0);
//    //    tempLU.rotate(Vec2(0, 0),  CC_DEGREES_TO_RADIANS(this->initialRotations[boneNamePrefix + "upper leg"]));
//    //    Vec2 tempP1 = upperLegInitialPos + tempLU;
//    //    this->debugDrawNode->drawLine(lowerLegInitialPos, tempP1, Color4F(.5f, 1.f, 1.f, 1.f));
//    //
//    //!!!debug
//    this->debugDrawNode->setLineWidth(1);
//    Vec2 temp(upperLegLength, 0);
//    temp.rotate(Vec2(0, 0), -alpha3);
//    Vec2 p3 = p0 + temp;
//    this->debugDrawNode->drawLine(p0 - this->getPosition(), p3 - this->getPosition(), Color4F(1.f, .5f, 1.f, 1.f));
//    Vec2 temp1 = lowerLegShift;
//    temp1.rotate(Vec2(), -alpha3);
//    Vec2 _p1 = p3 + temp1;
//    this->debugDrawNode->drawLine(_p1 - this->getPosition(), p3 - this->getPosition(), Color4F(0, 0, 0, 1.f));
//    Vec2 temp2(lowerLegLength, 0);
//    temp2.rotate(Vec2(0, 0), beta1);
//    Vec2 p4 = _p1 + temp2;
//    this->debugDrawNode->drawLine(_p1 - this->getPosition(), p4 - this->getPosition(), Color4F(.5f, 1.f, 1.f, 1.f));
//    Vec2 temp3 = footShift;
//    temp3.rotate(Vec2(), beta1);
//    Vec2 p5 = p4 + temp3;
//    this->debugDrawNode->drawLine(p4 - this->getPosition(), p5 - this->getPosition(), Color4F(1.f, 0, 0, 1.f));
//    this->debugDrawNode->drawLine(p0 - this->getPosition(), p2 - this->getPosition(), Color4F(1.f, 1.f, .5f, 1.f));
//
//    //    this->debugDrawNode->setScale(3);//!!!
//    this->debugDrawNode->setVisible(false);
//
//    Vec2 temp4(l0, 0);
//    temp4.rotate(Vec2(), -alpha3 + alpha4);
//    this->debugDrawNode->drawLine(p0 - this->getPosition(), p0 + temp4 - this->getPosition(), Color4F(0, 1.f, 0, .3f));
//
//    Vec2 temp5(l1, 0);
//    temp5.rotate(Vec2(), beta1 - beta2);
//    this->debugDrawNode->drawLine(p0 + temp4 - this->getPosition(), p0 + temp4 + temp5 - this->getPosition(), Color4F(0, 1.f, 0, .3f));
//
//    //    Vec2 temp6(0, -h);
//    //    temp6.rotate(Vec2::ZERO, globalRotation);
//    //    this->debugDrawNode->drawLine(p0 - this->getPosition(), p0 + temp6 - this->getPosition(), isP2LeftFromP0 ? Color4F(1.f, 1.f, 1.f, .7f) : Color4F(0.f, 0.f, 1.f, .7f));
//}

//
//    //Right lower leg
//    auto rightLowerLegBone = this->characterRender->findBone(RIGHT_LOWER_LEG);
//
//    float rightLowerLegBoneLength = getBoneLength(rightLowerLegBone, rightFootBone);
//    bodyShape.SetAsBox(rightLowerLegBoneLength, .03f);
//
//    auto rootBone = this->characterRender->findBone(ROOT);
//    Vec2 rightLowerLegBonePosition = getBonePosition(rightLowerLegBone, rootBone);
//    bodyDef.position.Set(rightLowerLegBonePosition.x, rightLowerLegBonePosition.y);
//    bodyDef.userData = (void*)&RIGHT_LOWER_LEG;
//
//    b2Body* rightLowerLegBody = world->CreateBody(&bodyDef);
//    rightLowerLegBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[RIGHT_LOWER_LEG] = rightLowerLegBody;
//
//    b2RevoluteJointDef footJointDef;
//    footJointDef.bodyA = rightLowerLegBody;
//    footJointDef.bodyB = rightFootBody;
//    footJointDef.localAnchorA = b2Vec2(rightLowerLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    //Left lower leg
//    auto leftLowerLegBone = this->characterRender->findBone(LEFT_LOWER_LEG);
//
//    float leftLowerLegBoneLength = getBoneLength(leftLowerLegBone, leftFootBone);
//    bodyShape.SetAsBox(leftLowerLegBoneLength, .03f);
//
//    Vec2 leftLowerLegBonePosition = getBonePosition(leftLowerLegBone, rootBone);
//    bodyDef.position.Set(leftLowerLegBonePosition.x, leftLowerLegBonePosition.y);
//    bodyDef.userData = (void*)&LEFT_LOWER_LEG;
//
//    b2Body* leftLowerLegBody = world->CreateBody(&bodyDef);
//    leftLowerLegBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[LEFT_LOWER_LEG] = leftLowerLegBody;
//
//    footJointDef.bodyA = leftLowerLegBody;
//    footJointDef.bodyB = leftFootBody;
//    footJointDef.localAnchorA = b2Vec2(leftLowerLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    //Right upper leg
//    auto rightUpperLegBone = this->characterRender->findBone(RIGHT_UPPER_LEG);
//
//    float rightUpperLegBoneLength = getBoneLength(rightUpperLegBone, rightLowerLegBone);
//    bodyShape.SetAsBox(rightUpperLegBoneLength, .03f);
//
//    Vec2 rightUpperLegBonePosition = getBonePosition(rightUpperLegBone, rootBone);
//    bodyDef.position.Set(rightUpperLegBonePosition.x, rightUpperLegBonePosition.y);
//    bodyDef.userData = (void*)&RIGHT_UPPER_LEG;
//
//    b2Body* rightUpperLegBody = world->CreateBody(&bodyDef);
//    rightUpperLegBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[RIGHT_UPPER_LEG] = rightUpperLegBody;
//
//    footJointDef.bodyA = rightUpperLegBody;
//    footJointDef.bodyB = rightLowerLegBody;
//    footJointDef.localAnchorA = b2Vec2(rightUpperLegBoneLength, 0.f);
//    footJointDef.localAnchorB = b2Vec2(-rightLowerLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    //Left upper leg
//    auto leftUpperLegBone = this->characterRender->findBone(LEFT_UPPER_LEG);
//
//    float leftUpperLegBoneLength = getBoneLength(leftUpperLegBone, leftLowerLegBone);
//    bodyShape.SetAsBox(leftUpperLegBoneLength, .03f);
//
//    Vec2 leftUpperLegBonePosition = getBonePosition(leftUpperLegBone, rootBone);
//    bodyDef.position.Set(leftUpperLegBonePosition.x, leftUpperLegBonePosition.y);
//    bodyDef.userData = (void*)&LEFT_UPPER_LEG;
//
//    b2Body* leftUpperLegBody = world->CreateBody(&bodyDef);
//    leftUpperLegBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[LEFT_UPPER_LEG] = leftUpperLegBody;
//
//    footJointDef.bodyA = leftUpperLegBody;
//    footJointDef.bodyB = leftLowerLegBody;
//    footJointDef.localAnchorA = b2Vec2(leftUpperLegBoneLength, 0.f);
//    footJointDef.localAnchorB = b2Vec2(-leftLowerLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    //Hip
//    auto hipBone = this->characterRender->findBone(HIP);
//
//    bodyShape.SetAsBox(.05f, .05f);
//
//    Vec2 hipBonePosition = getBonePosition(hipBone, rootBone);
//    bodyDef.position.Set(hipBonePosition.x, hipBonePosition.y);
//    bodyDef.userData = (void*)&HIP;
//
//    b2Body* hipBody = world->CreateBody(&bodyDef);
//    hipBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[HIP] = hipBody;
//
//    footJointDef.bodyA = hipBody;
//    footJointDef.bodyB = rightUpperLegBody;
//    footJointDef.localAnchorA = b2Vec2(0.f, 0.f);
//    footJointDef.localAnchorB = b2Vec2(-rightUpperLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    footJointDef.bodyA = hipBody;
//    footJointDef.bodyB = leftUpperLegBody;
//    footJointDef.localAnchorB = b2Vec2(-leftUpperLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);

//void Character::initPhysics()
//{
//    boneBodies.clear();
//
//    spSkeleton_updateWorldTransform(characterRender->getSkeleton());
//
//    auto world = static_cast<GameScreen*>(getScene())->getWorld();
//
//    //    b2BodyDef bodyDef;
//    //    bodyDef.type = b2_dynamicBody;
//    //    bodyDef.position.Set(this->getPosition().x + CENTER.x, this->getPosition().y + CENTER.y);
//    //    bodyDef.userData = this;
//    //    this->body = world->CreateBody(&bodyDef);
//    //
//    //    b2PolygonShape bodyShape;
//    //    bodyShape.SetAsBox(.2f, CENTER.y);
//
//    b2PolygonShape bodyShape;
//    bodyShape.SetAsBox(.1f, .05f);
//
//    b2FixtureDef bodyhapeDef;
//    bodyhapeDef.shape = &bodyShape;
//    bodyhapeDef.density = 50.f;
//    bodyhapeDef.friction = 20.6f;
//    bodyhapeDef.restitution = 0.f;//.01f;
//
//    //Right foot
//    auto rightFootBone = this->characterRender->findBone(RIGHT_FOOT);
//
//    b2BodyDef bodyDef;
//    bodyDef.type = b2_dynamicBody;
//    bodyDef.position.Set(this->getPosition().x - .2f, this->getPosition().y + .05f);
//    bodyDef.userData = (void*)&RIGHT_FOOT;
//
//    b2Body* rightFootBody = world->CreateBody(&bodyDef);
//    rightFootBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[RIGHT_FOOT] = rightFootBody;
//
//    //Left foot
//    auto leftFootBone = this->characterRender->findBone(LEFT_FOOT);
//
//    bodyDef.position.Set(this->getPosition().x + .2f, this->getPosition().y + .05f);
//    bodyDef.userData = (void*)&LEFT_FOOT;
//
//    b2Body* leftFootBody = world->CreateBody(&bodyDef);
//    leftFootBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[LEFT_FOOT] = leftFootBody;
//
//    //Right lower leg
//    auto rightLowerLegBone = this->characterRender->findBone(RIGHT_LOWER_LEG);
//
//    float rightLowerLegBoneLength = getBoneLength(rightLowerLegBone, rightFootBone);
//    bodyShape.SetAsBox(rightLowerLegBoneLength, .03f);
//
//    auto rootBone = this->characterRender->findBone(ROOT);
//    Vec2 rightLowerLegBonePosition = getBonePosition(rightLowerLegBone, rootBone);
//    bodyDef.position.Set(rightLowerLegBonePosition.x, rightLowerLegBonePosition.y);
//    bodyDef.userData = (void*)&RIGHT_LOWER_LEG;
//
//    b2Body* rightLowerLegBody = world->CreateBody(&bodyDef);
//    rightLowerLegBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[RIGHT_LOWER_LEG] = rightLowerLegBody;
//
//    b2RevoluteJointDef footJointDef;
//    footJointDef.bodyA = rightLowerLegBody;
//    footJointDef.bodyB = rightFootBody;
//    footJointDef.localAnchorA = b2Vec2(rightLowerLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    //Left lower leg
//    auto leftLowerLegBone = this->characterRender->findBone(LEFT_LOWER_LEG);
//
//    float leftLowerLegBoneLength = getBoneLength(leftLowerLegBone, leftFootBone);
//    bodyShape.SetAsBox(leftLowerLegBoneLength, .03f);
//
//    Vec2 leftLowerLegBonePosition = getBonePosition(leftLowerLegBone, rootBone);
//    bodyDef.position.Set(leftLowerLegBonePosition.x, leftLowerLegBonePosition.y);
//    bodyDef.userData = (void*)&LEFT_LOWER_LEG;
//
//    b2Body* leftLowerLegBody = world->CreateBody(&bodyDef);
//    leftLowerLegBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[LEFT_LOWER_LEG] = leftLowerLegBody;
//
//    footJointDef.bodyA = leftLowerLegBody;
//    footJointDef.bodyB = leftFootBody;
//    footJointDef.localAnchorA = b2Vec2(leftLowerLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    //Right upper leg
//    auto rightUpperLegBone = this->characterRender->findBone(RIGHT_UPPER_LEG);
//
//    float rightUpperLegBoneLength = getBoneLength(rightUpperLegBone, rightLowerLegBone);
//    bodyShape.SetAsBox(rightUpperLegBoneLength, .03f);
//
//    Vec2 rightUpperLegBonePosition = getBonePosition(rightUpperLegBone, rootBone);
//    bodyDef.position.Set(rightUpperLegBonePosition.x, rightUpperLegBonePosition.y);
//    bodyDef.userData = (void*)&RIGHT_UPPER_LEG;
//
//    b2Body* rightUpperLegBody = world->CreateBody(&bodyDef);
//    rightUpperLegBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[RIGHT_UPPER_LEG] = rightUpperLegBody;
//
//    footJointDef.bodyA = rightUpperLegBody;
//    footJointDef.bodyB = rightLowerLegBody;
//    footJointDef.localAnchorA = b2Vec2(rightUpperLegBoneLength, 0.f);
//    footJointDef.localAnchorB = b2Vec2(-rightLowerLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    //Left upper leg
//    auto leftUpperLegBone = this->characterRender->findBone(LEFT_UPPER_LEG);
//
//    float leftUpperLegBoneLength = getBoneLength(leftUpperLegBone, leftLowerLegBone);
//    bodyShape.SetAsBox(leftUpperLegBoneLength, .03f);
//
//    Vec2 leftUpperLegBonePosition = getBonePosition(leftUpperLegBone, rootBone);
//    bodyDef.position.Set(leftUpperLegBonePosition.x, leftUpperLegBonePosition.y);
//    bodyDef.userData = (void*)&LEFT_UPPER_LEG;
//
//    b2Body* leftUpperLegBody = world->CreateBody(&bodyDef);
//    leftUpperLegBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[LEFT_UPPER_LEG] = leftUpperLegBody;
//
//    footJointDef.bodyA = leftUpperLegBody;
//    footJointDef.bodyB = leftLowerLegBody;
//    footJointDef.localAnchorA = b2Vec2(leftUpperLegBoneLength, 0.f);
//    footJointDef.localAnchorB = b2Vec2(-leftLowerLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    //Hip
//    auto hipBone = this->characterRender->findBone(HIP);
//
//    bodyShape.SetAsBox(.05f, .05f);
//
//    Vec2 hipBonePosition = getBonePosition(hipBone, rootBone);
//    bodyDef.position.Set(hipBonePosition.x, hipBonePosition.y);
//    bodyDef.userData = (void*)&HIP;
//
//    b2Body* hipBody = world->CreateBody(&bodyDef);
//    hipBody->CreateFixture(&bodyhapeDef);
//    this->boneBodies[HIP] = hipBody;
//
//    footJointDef.bodyA = hipBody;
//    footJointDef.bodyB = rightUpperLegBody;
//    footJointDef.localAnchorA = b2Vec2(0.f, 0.f);
//    footJointDef.localAnchorB = b2Vec2(-rightUpperLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//
//    footJointDef.bodyA = hipBody;
//    footJointDef.bodyB = leftUpperLegBody;
//    footJointDef.localAnchorB = b2Vec2(-leftUpperLegBoneLength, 0.f);
//
//    world->CreateJoint(&footJointDef);
//}