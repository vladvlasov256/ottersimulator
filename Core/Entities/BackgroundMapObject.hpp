//
//  BackgroundMapObject.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#ifndef BackgroundMapObject_hpp
#define BackgroundMapObject_hpp

#include "Entity.hpp"

namespace ottersimulator
{
    class BackgroundMapObject : public Entity
    {
    private:
        cocos2d::Sprite* sprite;
    protected:
        void setSprite(const std::string& fileName);
    public:
        virtual bool init() override;
        virtual bool initWithFile(const std::string& fileName);
        
        virtual bool loadImpl(const rapidjson::Value& json) override;
        virtual bool saveImpl(rapidjson::Document& document, rapidjson::Value& json) override;
//        virtual bool isTouched(const Vec2& pos) const override;
        
        static BackgroundMapObject* create(const std::string& fileName);
        
        CREATE_FUNC(BackgroundMapObject);
    };
}

#endif /* BackgroundMapObject_hpp */
