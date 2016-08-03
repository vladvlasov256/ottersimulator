//
//  ObstacleMapObject.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#ifndef DynamicMapObject_hpp
#define DynamicMapObject_hpp

#include "Entity.hpp"

namespace ottersimulator
{
    class DynamicObject;
    
    class DynamicMapObject : public Entity
    {
    private:
        std::string fileName;
        cocos2d::Sprite* sprite;
        DynamicObject* dynamicObject;
    protected:
        void setFileName(const std::string& fileName);
        void setSprite(const std::string& fileName);
    public:
        virtual bool init() override;
        virtual bool initWithFile(const std::string& fileName);
        
        const DynamicObject* getDynamicObject() const { return dynamicObject; }
        
        virtual bool loadImpl(const rapidjson::Value& json) override;
        virtual bool saveImpl(rapidjson::Document& document, rapidjson::Value& json) override;
        
        CREATE_FUNC(DynamicMapObject);
        static DynamicMapObject* create(const std::string& fileName);
    };
}

#endif /* ObstacleMapObject_hpp */
