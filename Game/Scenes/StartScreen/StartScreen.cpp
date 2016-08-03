//
//  StartScreen.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#include "MainMenuLayer.hpp"
#include "LevelSelectionMenuLayer.hpp"
#include "EditorStartMenuLayer.hpp"
#include "EditorLevelSelectionLayer.hpp"
#include "StartScreen.hpp"

USING_NS_CC;
using namespace ottersimulator;

bool StartScreen::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    this->backgroundLayer = Layer::create();
    this->addChild(this->backgroundLayer);
    
    this->mainMenuLayer = MainMenuLayer::create();
    this->presentMainMenuLayer();
    
    this->addChild(mainMenuLayer);
    this->presentedMenus.push(mainMenuLayer);
    
    this->levelSelectionMenuLayer = LevelSelectionMenuLayer::create();
    this->levelSelectionMenuLayer->retain();
    
    return true;
}

void StartScreen::setBackground(const std::string& image)
{
    if(this->background)
    {
        this->backgroundLayer->removeChild(this->background);
    }
    
    this->background = Sprite::create(image);
    
    auto size = Director::getInstance()->getWinSize();
    this->background->setPosition(Point(size.width / 2, size.height / 2));
    
    Vec2 scale = Vec2(size.width / background->getContentSize().width, size.height / background->getContentSize().height);
    this->background->setScale(MAX(scale.x, scale.y));
    
    this->backgroundLayer->addChild(background, 0);
}

void StartScreen::presentMainMenuLayer()
{
    this->setBackground("StartScreen/Background.jpg");
}

void StartScreen::presentLevelSelectionLayer()
{
    presentedMenus.top()->retain();
    removeChild(presentedMenus.top());
    
    addChild(levelSelectionMenuLayer);
    levelSelectionMenuLayer->release();
    
    presentedMenus.push(levelSelectionMenuLayer);
    
    levelSelectionMenuLayer->onMenuDidPresented();
}

void StartScreen::popMenuLayer()
{
    presentedMenus.top()->retain();
    removeChild(presentedMenus.top());
    
    presentedMenus.pop();
    
    addChild(presentedMenus.top());
    presentedMenus.top()->release();
    
    if(this->presentedMenus.top() == this->mainMenuLayer)
    {
        this->presentMainMenuLayer();
    }
}