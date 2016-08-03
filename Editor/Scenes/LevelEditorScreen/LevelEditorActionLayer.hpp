//
//  LevelEditorActionLayer.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#ifndef LevelEditorActionLayer_hpp
#define LevelEditorActionLayer_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "Entity.hpp"
#include "Landscape.hpp"

namespace ottersimulator
{
    class LandscapeControlPointMark : public cocos2d::ui::Widget
    {
    private:
        ControlPoint* controlPoint;
        cocos2d::Point lastTouchPosition;
        cocos2d::ui::Button* button;
        cocos2d::Sprite* sprite;
        cocos2d::Sprite* activeSprite;
    protected:
//        virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
//        virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
//        virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
//        virtual bool hitTest(const cocos2d::Vec2 &pt, const cocos2d::Camera* camera, cocos2d::Vec3 *p) const override;
    public:
        virtual bool initWithControlPoint(ControlPoint* controlPoint);
        
        ControlPoint* getControlPoint() const { return controlPoint; }
        void update();
        bool isTouched(const cocos2d::Vec2& pos) const;
        void setActive();
        void setInactive();
        void setHeight(const float height);
        
        static LandscapeControlPointMark* create(ControlPoint* controlPoint);
    };
    
    class LevelEditorActionLayer : public cocos2d::Layer
    {
    private:
        cocos2d::Vec2 scrollLastPosition;
        std::vector<LandscapeControlPointMark*> controlPointMarks;
        LandscapeControlPointMark* selectedControlPointEditMark;
        Entity* selectedEntity;
        bool hasLongPressUsed;
        cocos2d::EventListenerTouchOneByOne* touchListener;
        cocos2d::EventListenerMouse* mouseListener;
    protected:
        virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
        virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
        virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* pEvent) override;
        void onMouseScroll(cocos2d::EventMouse* event);
        void longPressTimer(float dt);
    public:
        virtual bool init() override;
        virtual void setVisible(bool isVisible) override;
        
        void enableControlPointEditing();
        void disableControlPointEditing();
        
        void update();
        void updateControlPointMarks();
        
        CREATE_FUNC(LevelEditorActionLayer);
    };
}

#endif /* LevelEditorActionLayer_hpp */
