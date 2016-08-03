//
//  LevelEditorGUILayer.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#include "global.h"
#include "LevelEditorScreen.hpp"
#include "LevelEditorGUILayer.hpp"
#include "ResourceManager.hpp"
#include "DynamicObjectManager.hpp"

USING_NS_CC;
using namespace ui;
using namespace ottersimulator;

enum EditorMenuTabs
{
    kEditorMenuTabLandscape = 0,
    kEditorMenuTabStaticObjects,
    kEditorMenuTabDynamicObjects,
    kEditorMenuTabBackground
};

bool LevelEditorGUILayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto button = Button::create();
    button->setTitleText("Menu");
    button->setTouchEnabled(true);
    button->setPosition(Point(Director::getInstance()->getWinSize().width * .975f, Director::getInstance()->getWinSize().height * .975f));
    button->setTitleFontSize(MENU_BUTTON_TITLE_FONT_SIZE * .25f);
    button->getTitleRenderer()->enableShadow();
    
    button->addTouchEventListener(CC_CALLBACK_2(LevelEditorGUILayer::onMenuButtonTouch, this));
    
    addChild(button);
    
    //!!!!todo: undo, redo
    
//    auto landscapeBarButton = Button::create("Textures/GUI/Editor/landscapeTabBarButton.png", "Textures/GUI/Editor/landscapeTabBarButton.png");
//    tabBarButtons.push_back(landscapeBarButton);
    tabBarButtonResources.push_back("Textures/GUI/Editor/landscapeTabBarButton.png");
    
//    auto staticObjectsBarButton = Button::create("Textures/GUI/Editor/staticObjectsTabBarButton.png", "Textures/GUI/Editor/staticObjectsTabBarButton.png");
//    tabBarButtons.push_back(staticObjectsBarButton);
    tabBarButtonResources.push_back("Textures/GUI/Editor/staticObjectsTabBarButton.png");
    
//    auto dynamicObjectsBarButton = Button::create("Textures/GUI/Editor/dynamicObjectsTabBarButton.png", "Textures/GUI/Editor/dynamicObjectsTabBarButton.png");
//    tabBarButtons.push_back(dynamicObjectsBarButton);
    tabBarButtonResources.push_back("Textures/GUI/Editor/dynamicObjectsTabBarButton.png");
    
//    auto backgroundBarButton = Button::create("Textures/GUI/Editor/backgroundTabBarButton.png", "Textures/GUI/Editor/backgroundTabBarButton.png");
//    tabBarButtons.push_back(backgroundBarButton);
    tabBarButtonResources.push_back("Textures/GUI/Editor/backgroundTabBarButton.png");
    
//    auto landscapeHeightEditorTool = Button::create("Textures/GUI/Editor/landscapeHeightEditorTool.png", "Textures/GUI/Editor/landscapeHeightEditorTool.png");
//    landscapeToolButtons.push_back(landscapeHeightEditorTool);
    landscapeToolButtonResources.push_back("Textures/GUI/Editor/landscapeHeightEditorTool.png");
    
//    auto landscapeAppendLeftTool = Button::create("Textures/GUI/Editor/landscapeAppendLeftTool.png", "Textures/GUI/Editor/landscapeAppendLeftTool.png");
//    landscapeToolButtons.push_back(landscapeAppendLeftTool);
    landscapeToolButtonResources.push_back("Textures/GUI/Editor/landscapeAppendLeftTool.png");
    
//    auto landscapeAppendRightTool = Button::create("Textures/GUI/Editor/landscapeAppendRightTool.png", "Textures/GUI/Editor/landscapeAppendRightTool.png");
//    landscapeToolButtons.push_back(landscapeAppendRightTool);
    landscapeToolButtonResources.push_back("Textures/GUI/Editor/landscapeAppendRightTool.png");
    
    tabMenu = EditorTabMenu::create();
    tabMenu->setDelegate(this);
    tabMenu->setPosition(Vec2(Director::getInstance()->getWinSize().width * .5f, Director::getInstance()->getWinSize().height * .125f));
    tabMenu->setContentSize(Size(Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height * .25f));
    
    addChild(tabMenu);
    
    return true;
}

void LevelEditorGUILayer::onMenuButtonTouch(Ref* pSender, Widget::TouchEventType touchEventType)
{
    if(touchEventType == Widget::TouchEventType::ENDED)
    {
        ((LevelEditorScreen*)getScene())->presentEditorMenuLayer();
    }
}

Widget* LevelEditorGUILayer::getThumbnailForTabBarButton(size_t tabIndex)
{
//    return tabBarButtons[tabIndex];
    return Button::create(tabBarButtonResources[tabIndex], tabBarButtonResources[tabIndex]);
}

size_t LevelEditorGUILayer::getItemNumForTab(size_t tabIndex)
{
    switch (tabIndex)
    {
        case kEditorMenuTabLandscape:
            return landscapeToolButtonResources.size();
        case kEditorMenuTabStaticObjects:
            return ResourceManager::getInstance()->getNumResources(ResourceManager::kStaticObjects);
        case kEditorMenuTabDynamicObjects:
            return ResourceManager::getInstance()->getNumResources(ResourceManager::kDynamicObjects);
        case kEditorMenuTabBackground:
            return ResourceManager::getInstance()->getNumResources(ResourceManager::kBackgrounds);
    }
    
    return 0;
}

Widget* getThumbnailForResource(const ResourceManager::ResourceType resourceType, const size_t resourceIndex)
{
    std::string filePath;
    switch (resourceType)
    {
        case ResourceManager::kStaticObjects:
            filePath = ResourceManager::getInstance()->getStaticObjectFilePath(resourceIndex);
            break;
        case ResourceManager::kBackgrounds:
            filePath = ResourceManager::getInstance()->getBackgroundFilePath(resourceIndex);
            break;
        case ResourceManager::kDynamicObjects:
            filePath = DynamicObjectManager::getInstance()->getTextureFilePath(resourceIndex);
            break;
        default:
            CC_ASSERT(false);
    }
    
    return Button::create(filePath, filePath);
}

Widget* LevelEditorGUILayer::getThumbnailForItem(size_t tabIndex, size_t itemIndex)
{
    switch (tabIndex)
    {
        case kEditorMenuTabLandscape:
//            return landscapeToolButtons[itemIndex];
            return Button::create(landscapeToolButtonResources[itemIndex], landscapeToolButtonResources[itemIndex]);
        case kEditorMenuTabStaticObjects:
            return getThumbnailForResource(ResourceManager::kStaticObjects, itemIndex);
        case kEditorMenuTabDynamicObjects:
            return getThumbnailForResource(ResourceManager::kDynamicObjects, itemIndex);
        case kEditorMenuTabBackground:
            return getThumbnailForResource(ResourceManager::kBackgrounds, itemIndex);
    };
    
    return NULL;
}

void LevelEditorGUILayer::onItemDidSelected(size_t tabIndex, size_t itemIndex)
{
    auto scene = static_cast<LevelEditorScreen*>(getScene());
    
    switch (tabIndex)
    {
        case kEditorMenuTabLandscape:
            {
                switch (itemIndex)
                {
                    case 0:
                        scene->enableControlPointEditing();
                        break;
                        
                    case 1:
                        scene->addBlockToTheLeft();
                        break;
                    
                    case 2:
                        scene->addBlockToTheRight();
                        break;
                        
                    default:
                        break;
                }
                
            }
            break;
        case kEditorMenuTabStaticObjects:
            scene->cretateBackgroundMapObject(ResourceManager::getInstance()->getStaticObjectFilePath(itemIndex));
        case kEditorMenuTabDynamicObjects:
            scene->cretateDynamicMapObject(ResourceManager::getInstance()->getDynamicObjectFilePath(itemIndex));
            break;
        case kEditorMenuTabBackground:
            scene->getMap()->setBackgroundSpriteTextureFileName(ResourceManager::getInstance()->getBackgroundFilePath(itemIndex));
            break;
    }
}