//
//  Longboard.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 02.05.16.
//
//

#include "global.h"
#include "Longboard.hpp"
#include "GameScreen.hpp"
#include "Map.hpp"

USING_NS_CC;
using namespace ottersimulator;

bool Longboard::initWithTexureAndWheel(const std::string& textureFileName, const std::string& wheelFileName)
{
    if(!Node::init())
    {
        return false;
    }
    
    Size windowSize = Director::getInstance()->getWinSize();
    
    float scale = UNITS_IN_SCREEN_HEIGHT / windowSize.height * Director::getInstance()->getContentScaleFactor() * .5f;
    
    this->board = Sprite::create(textureFileName);
    this->board->setScale(scale);
    addChild(this->board);
    
    this->backWheel = Sprite::create(wheelFileName);
    this->backWheel->setPosition(Vec2(-.39f, -.06f));
    this->backWheel->setScale(scale);
    addChild(this->backWheel);
    
    this->frontWheel = Sprite::create(wheelFileName);
    this->frontWheel->setPosition(Vec2(.44f, -.06f));
    this->frontWheel->setScale(scale);
    addChild(this->frontWheel);
    
//    this->initPhysics();
    
    return true;
}

Longboard* Longboard::createWithTexureAndWheel(const std::string& textureFileName, const std::string& wheelFileName)
{
    Longboard *longboard = new (std::nothrow) Longboard();
    if (longboard && longboard->initWithTexureAndWheel(textureFileName, wheelFileName))
    {
        longboard->autorelease();
        return longboard;
    }
    CC_SAFE_DELETE(longboard);
    return nullptr;
}

void Longboard::initPhysics()
{
    auto world = static_cast<GameScreen*>(getScene())->getWorld();
    
    //Board
    b2BodyDef boardBodyDef;
    boardBodyDef.type = b2_dynamicBody;
    boardBodyDef.position.Set(this->getPosition().x + this->board->getPosition().x, this->getPosition().y + this->board->getPosition().y);
    boardBodyDef.userData = this->board;
    this->boardBody = world->CreateBody(&boardBodyDef);
    
    b2PolygonShape boardShape;
    boardShape.SetAsBox(.5f, .01f);
    
    b2FixtureDef boardShapeDef;
    boardShapeDef.shape = &boardShape;
    boardShapeDef.density = 25.0f;
    boardShapeDef.friction = 10.9f;
    boardShapeDef.restitution = 0.f;//.002f;
    this->boardBody->CreateFixture(&boardShapeDef);
    
    //Back wheel
    b2BodyDef backWheelDef;
    backWheelDef.type = b2_dynamicBody;
    backWheelDef.position.Set(this->getPosition().x + this->backWheel->getPosition().x, this->getPosition().y + this->backWheel->getPosition().y);
    backWheelDef.userData = this->backWheel;
    this->backWheelBody = world->CreateBody(&backWheelDef);
    
    b2CircleShape backWheelShape;
    backWheelShape.m_radius = .05f;
    
    b2FixtureDef backWheelShapeDef;
    backWheelShapeDef.shape = &backWheelShape;
    backWheelShapeDef.density = 60.f;
    backWheelShapeDef.friction = .9f;
    backWheelShapeDef.restitution = 0.f;//.002f;
    this->backWheelBody->CreateFixture(&backWheelShapeDef);
    
    //Front wheel
    b2BodyDef frontWheelDef;
    frontWheelDef.type = b2_dynamicBody;
    frontWheelDef.position.Set(this->getPosition().x + this->frontWheel->getPosition().x, this->getPosition().y + this->frontWheel->getPosition().y);
    frontWheelDef.userData = this->frontWheel;
    this->frontWheelBody = world->CreateBody(&frontWheelDef);
    
    b2CircleShape frontWheelShape;
    frontWheelShape.m_radius = .05f;
    
    b2FixtureDef frontWheelShapeDef;
    frontWheelShapeDef.shape = &frontWheelShape;
    frontWheelShapeDef.density = 60.f;
    frontWheelShapeDef.friction = .9f;
    frontWheelShapeDef.restitution = 0.f;//.002f;
    this->frontWheelBody->CreateFixture(&frontWheelShapeDef);
    
    //Back joint
    b2RevoluteJointDef backJointDef;
    backJointDef.bodyA = this->boardBody;
    backJointDef.bodyB = this->backWheelBody;
    backJointDef.localAnchorA = b2Vec2(this->backWheel->getPosition().x, this->backWheel->getPosition().y);
    
    this->backJoint = (b2RevoluteJoint*)world->CreateJoint(&backJointDef);
    
    //Front joint
    b2RevoluteJointDef frontJointDef;
    frontJointDef.bodyA = this->boardBody;
    frontJointDef.bodyB = this->frontWheelBody;
    frontJointDef.localAnchorA = b2Vec2(this->frontWheel->getPosition().x, this->frontWheel->getPosition().y);
    
    this->frontJoint = (b2RevoluteJoint*)world->CreateJoint(&frontJointDef);
}

void Longboard::updateSprite(b2Body* body)
{
    if(body == this->boardBody)
    {
        this->setPosition(body->GetPosition().x, body->GetPosition().y);
        this->setRotation(-CC_RADIANS_TO_DEGREES(body->GetAngle()));
    }
    else if(body == this->backWheelBody)
    {
        this->backWheel->setRotation(-CC_RADIANS_TO_DEGREES(body->GetAngle()));///!!!!
    }
    else if(body == this->frontWheelBody)
    {
        this->frontWheel->setRotation(-CC_RADIANS_TO_DEGREES(body->GetAngle()));///!!!!
    }
}

void Longboard::applyPush()
{
    Vec2 force = Vec2(2.f, 0.f);
    force.rotate(Vec2(), CC_DEGREES_TO_RADIANS(this->getRotation()));
    this->boardBody->ApplyLinearImpulse(b2Vec2(force.x, force.y), this->boardBody->GetWorldCenter(), true);
//    this->boardBody->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void Longboard::applyBrake(float elapsedTime)
{
//    b2Vec2 velocity = this->boardBody->GetLinearVelocityFromLocalPoint(b2Vec2(0, 0));
    b2Vec2 velocity = this->boardBody->GetLinearVelocity();
    printf("velocity = %.3f\n", velocity.Length());
    if(velocity.Length() < .52f)
    {
        velocity *= -1.f;
        this->boardBody->ApplyLinearImpulse(velocity, this->boardBody->GetWorldCenter(), true);
        
        this->blockWheelJoint(this->backJoint);
        this->blockWheelJoint(this->frontJoint);
    }
    else
    {
        //!!!todo: use square root or something
        velocity *= -10.f * elapsedTime;
        this->boardBody->ApplyLinearImpulse(velocity, this->boardBody->GetWorldCenter(), true);
    //    this->boardBody->ApplyForceToCenter(velocity, true);
    }
}

void Longboard::blockWheelJoint(b2RevoluteJoint* joint)
{
    if(!joint->IsLimitEnabled())
    {
        joint->EnableLimit(true);
        float angle = joint->GetJointAngle();
        joint->SetLimits(angle - .1f, angle + .1f);
    }
}

void Longboard::endBrake()
{
    if(this->backJoint->IsLimitEnabled())
    {
        this->backJoint->EnableLimit(false);
    }
    
    if(this->frontJoint->IsLimitEnabled())
    {
        this->frontJoint->EnableLimit(false);
    }
}