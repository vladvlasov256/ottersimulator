//
//  Entity.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#ifndef Entity_hpp
#define Entity_hpp

#include "cocos2d.h"
#include "rapidjson/document.h"

namespace ottersimulator
{    
    class Entity : public cocos2d::Node
    {
    public:
        typedef std::vector<cocos2d::Node>::iterator NodeIterator;
    private:
        cocos2d::Size size;
        cocos2d::Rect bounds;
        
        static Entity* LoadFromJSON(const rapidjson::Value& json);
        void SaveToJSON(rapidjson::Document& document, rapidjson::Value& json);
    protected:
        std::string typeName;
        
        virtual bool loadImpl(const rapidjson::Value& json);
        virtual bool saveImpl(rapidjson::Document& document, rapidjson::Value& json);
        virtual void setSize(const cocos2d::Size& size);
        virtual void updateBounds();
        virtual void tick() {};
    public:
        static Entity* LoadFromFile(const std::string& fileName);
        void SaveToFile(const std::string& fileName);
        virtual bool init() override;
        
        const std::string& getTypeName() const { return typeName; }
        
        virtual const cocos2d::Size& getSize() const { return size; }
        virtual bool isTouched(const cocos2d::Vec2& pos) const;
        virtual const cocos2d::Rect& getBounds() const { return bounds; }
        virtual void setPosition(const cocos2d::Vec2 &pos) override;
        
        CREATE_FUNC(Entity);
    };
}

#endif /* Entity_hpp */
