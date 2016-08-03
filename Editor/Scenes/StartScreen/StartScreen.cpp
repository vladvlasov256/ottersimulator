//
//  StartScreen.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

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
    
    addChild(createBackgroundLayer());
    
    editorStartMenuLayer = EditorStartMenuLayer::create();
    
    addChild(editorStartMenuLayer);
    presentedMenus.push(editorStartMenuLayer);
    
    editorLevelSelectionLayer = EditorLevelSelectionLayer::create();
    editorLevelSelectionLayer->retain();
    
    return true;
}

Layer* StartScreen::createBackgroundLayer()
{
    auto background = Sprite::create("StartScreen/Background.jpg");
    
    auto size = Director::getInstance()->getWinSize();
    background->setPosition(Point(size.width / 2, size.height / 2));
    background->setScale(size.height / background->getContentSize().height);
    
    auto layer = Layer::create();
    layer->addChild(background, 0);
    
    return layer;
}

void StartScreen::presentEditorLevelSelectionLayer()
{
    presentedMenus.top()->retain();
    removeChild(presentedMenus.top());
    
    addChild(editorLevelSelectionLayer);
    editorLevelSelectionLayer->release();
    
    presentedMenus.push(editorLevelSelectionLayer);
    
    editorLevelSelectionLayer->onMenuDidPresented();
}

void StartScreen::popMenuLayer()
{
    presentedMenus.top()->retain();
    removeChild(presentedMenus.top());
    
    presentedMenus.pop();
    
    addChild(presentedMenus.top());
    presentedMenus.top()->release();
}