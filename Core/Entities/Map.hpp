//
//  Level.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#ifndef Map_hpp
#define Map_hpp

#include "Entity.hpp"
#include "Landscape.hpp"

namespace ottersimulator
{
    class BackgroundMapObject;
    class DynamicMapObject;
    
    typedef std::vector<DynamicMapObject*> DynamicMapObjects;
    typedef DynamicMapObjects::const_iterator DynamicMapObjectsIt;
    
    class Map : public Entity
    {
    private:
        Landscape *landscape;
        cocos2d::Sprite *backgroundSprite;
        const std::string levelFilePath; //???
        std::vector<BackgroundMapObject*> backgroundObjects;
        std::vector<DynamicMapObject*> dynamicObjects;
        cocos2d::Rect visibleRect;
    protected:
        virtual bool loadImpl(const rapidjson::Value& json) override;
        virtual bool saveImpl(rapidjson::Document& document, rapidjson::Value& json) override;
        void updateBackground();
        void addLandscape(Landscape* landscape);
        void addBackgroundMapObject(BackgroundMapObject* backgroundObject);
        void addDynamicMapObject(DynamicMapObject* dynamicObject);
    public:
        virtual bool init() override;
        virtual void addChild(cocos2d::Node * child) override;
        
        cocos2d::Sprite *getBackgroundSprite() const { return backgroundSprite; }
        void setBackgroundSpriteTextureFileName(const std::string& fileName);
        
        Landscape* createFlatLandscape();
        Landscape* getLandscape() const { return landscape; }
        
        BackgroundMapObject* addBackgroundMapObject(const std::string& fileName);
        
        DynamicMapObject* addDynamicMapObject(const std::string& fileName);
        DynamicMapObjectsIt getDynamicMapObjectsBegin() const { return dynamicObjects.begin(); }
        DynamicMapObjectsIt getDynamicMapObjectsEnd() const { return dynamicObjects.end(); }
        
        const cocos2d::Rect& getVisibleRect() const { return visibleRect; }
        void setVisibleRect(const cocos2d::Rect& rect);
        
        Entity* getTouchedObject(const cocos2d::Vec2& pos) const;
        
        CREATE_FUNC(Map);
    };
}


#endif /* Level_hpp */
