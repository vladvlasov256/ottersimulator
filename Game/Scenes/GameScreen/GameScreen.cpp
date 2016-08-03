//
//  GameScreen.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#include "global.h"
#include "GameScreen.hpp"
#include "LevelManager.hpp"
#include "B2DebugDrawLayer.h"
#include "DynamicObjectManager.hpp"
#include "DynamicMapObject.hpp"

USING_NS_CC;
using namespace ottersimulator;

bool GameScreen::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    this->actionLayer = GameActionLayer::create();
    this->addChild(this->actionLayer);
    
    this->hudLayer = GameHUDLayer::create();
    this->addChild(this->hudLayer);
    
    this->zoom = 1.f;
    
    this->world = new b2World(b2Vec2(0.0f, -9.8f));
    
    this->longboard = Longboard::createWithTexureAndWheel("Longboard/longboard.png", "Longboard/offroad_wheel.png");
    this->character = Character::createWithSkeletonAndAtlas("Character/spineboy-skeleton.json", "Character/spineboy.atlas");
    
    this->physicsDebugLayer = B2DebugDrawLayer::create(this->world, 1.f);
    this->physicsDebugLayer->retain();
    
    return true;
}

GameScreen::~GameScreen()
{
    delete world;
}

void GameScreen::loadMap(size_t mapIndex)
{
    CC_ASSERT(!this->map);
    
    this->mapIndex = mapIndex;
    
    this->map = (ottersimulator::Map*)Entity::LoadFromFile(LevelManager::getInstance()->getLevelFilePath(this->mapIndex));
    this->addChild(this->map, -1);
    
    Point windowSize = Director::getInstance()->getWinSize();
    focus = Vec2(windowSize.x / windowSize.y, 1) * .5f * UNITS_IN_SCREEN_HEIGHT;
    
    auto landscape = this->map->getLandscape();
    if(landscape)
    {
        this->map->addChild(this->longboard);
        this->longboard->setPosition(Vec2(1.f, landscape->getHeightAtPoint(1.f) + 0.1f));
        
        this->map->addChild(this->character);
        this->character->setPosition(Vec2(1.1f, landscape->getHeightAtPoint(1.f) + 0.13f));
        
        focus = character->getPosition() + Vec2(windowSize.x / windowSize.y * .5f * UNITS_IN_SCREEN_HEIGHT - 1.f, 0);
    }
    
    this->initPhysics();
    
//    this->map->setVisible(false);
    
    this->updateVisibleRect();
    
    this->physicsDebugLayer->release();
    this->map->Node::addChild(this->physicsDebugLayer, 99);
    this->physicsDebugLayer->setVisible(this->enableDebugPhysicsDraw);
    
    schedule(CC_SCHEDULE_SELECTOR(GameScreen::tick), .02f);
}

void GameScreen::updateVisibleRect()
{
    Point windowSize = Director::getInstance()->getWinSize();
    Vec2 visibleRectSize = Vec2(windowSize.x / windowSize.y, 1) * UNITS_IN_SCREEN_HEIGHT / this->zoom;
    Rect rect(this->focus.x - visibleRectSize.x * .5f, this->focus.y - visibleRectSize.y * .5f, visibleRectSize.x, visibleRectSize.y);
    this->map->setVisibleRect(rect);
}

void GameScreen::onPushButtonTouchBegin()
{
    this->character->startPush();
}

void GameScreen::onPushButtonTouchEnd()
{
    this->character->endPush();
}

void GameScreen::tick(float dt)
{
    this->world->Step(dt, 10, 10);
    
    for(b2Body *body = this->world->GetBodyList(); body; body = body->GetNext())
    {
        ///!!!!implement system
        this->longboard->updateSprite(body);
        this->character->updateSprite(body);//!!!save body list???
    }
    
    this->character->onTick();
    
    Point windowSize = Director::getInstance()->getWinSize();
    focus = character->getPosition() + Vec2(windowSize.x / windowSize.y * .5f * UNITS_IN_SCREEN_HEIGHT - 1.f, 0);
    updateVisibleRect();
}

void GameScreen::initPhysics()
{
    this->initLandscapePhysics();
    this->initDynamicObjectsPhysics();
    
    this->longboard->initPhysics();
    this->character->initPhysics();
    this->character->putOnLongboard(this->longboard);
}

void GameScreen::initLandscapePhysics()
{
    auto landscape = map->getLandscape();
    if(landscape)
    {
        //!!!move into landscape class
        b2BodyDef landscapeBodyDef;
        landscapeBodyDef.type = b2_staticBody;
        landscapeBodyDef.position.Set(0, 0);
        landscapeBodyDef.userData = landscape;
        this->landscapeBody = this->world->CreateBody(&landscapeBodyDef);
        
        b2EdgeShape landscapeShape;
        b2FixtureDef landscapeDef;
        landscapeDef.friction = .9f;
        landscapeDef.restitution = 0.f;//.002f;
        landscapeDef.shape = &landscapeShape;
        
        Rect landscapeBounds = landscape->getBounds();
        float startX = landscapeBounds.getMinX();
        float startY = landscape->getHeightAtPoint(startX);
        for(float endX = startX + .2f; endX <= landscapeBounds.getMaxX(); endX += .2f)///!!!magic const
        {
            float endY = landscape->getHeightAtPoint(endX);
            
            landscapeShape.Set(b2Vec2(startX, startY), b2Vec2(endX, endY));
            landscapeBody->CreateFixture(&landscapeDef);
            
            startX = endX;
            startY = endY;
        }
    }
}

void GameScreen::initDynamicObjectsPhysics()
{
    for(DynamicMapObjectsIt it = this->map->getDynamicMapObjectsBegin(); it != this->map->getDynamicMapObjectsEnd(); it++)
    {
        auto dynamicMapObject = *it;
        
        Size windowSize = Director::getInstance()->getWinSize();
        const Vec2 size = dynamicMapObject->getSize();
        const Vec2 scaleVec(1.f, 1.f);// * (UNITS_IN_SCREEN_HEIGHT / windowSize.height);
        
        //!!!move into dynamic object class
        b2BodyDef dynamicObjectBodyDef;
        dynamicObjectBodyDef.type = b2_staticBody;
        dynamicObjectBodyDef.position.Set(dynamicMapObject->getPosition().x - size.x * .5f, dynamicMapObject->getPosition().y - size.y * .5f);
        dynamicObjectBodyDef.userData = dynamicMapObject;
        auto body = this->world->CreateBody(&dynamicObjectBodyDef);
        
        DynamicObject::PolygonsIt polIt = dynamicMapObject->getDynamicObject()->getPolygonsBegin();
        DynamicObject::PolygonsIt polItEnd = dynamicMapObject->getDynamicObject()->getPolygonsEnd();
        for(; polIt != polItEnd; polIt++)
        {
            const DynamicObject::Polygon& polygon = *polIt;
            
            b2Vec2* vertices = (b2Vec2*)malloc(polygon.size() * sizeof(b2Vec2));
            for(size_t iVertex = 0; iVertex < polygon.size(); iVertex++)
            {
                vertices[iVertex] = b2Vec2(polygon[iVertex].x * size.x, polygon[iVertex].y * size.x);
            }
            
            b2PolygonShape polygonShape;
            polygonShape.Set(vertices, polygon.size());
            
            free(vertices);
            
            b2FixtureDef polygonShapeDef;
            polygonShapeDef.shape = &polygonShape;
            
            body->CreateFixture(&polygonShapeDef);
        }
    }
}

void GameScreen::onChangeEnableDebugDrawPhysics()
{
    this->enableDebugPhysicsDraw = !this->enableDebugPhysicsDraw;
    this->physicsDebugLayer->setVisible(this->enableDebugPhysicsDraw);
}