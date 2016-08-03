//
//  DynamicObjectManager.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 31.05.16.
//
//

#ifndef DynamicObjectManager_hpp
#define DynamicObjectManager_hpp

#include "cocos2d.h"

namespace ottersimulator
{
    class DynamicObject : public cocos2d::Ref
    {
    public:
        typedef std::vector<cocos2d::Vec2> Polygon;
        typedef std::vector<Polygon>::const_iterator PolygonsIt;
    private:
        std::string fileName;
        std::string textureFilePath;
        std::vector<Polygon> polygons;
    public:
        bool initWithFileName(const std::string& fileName);
        
        const std::string& getTextureFilePath() const { return textureFilePath; }
        const std::string& getFileName() const { return fileName; }
        PolygonsIt getPolygonsBegin() const { return polygons.cbegin(); }
        PolygonsIt getPolygonsEnd() const { return polygons.cend(); }
        
        static DynamicObject* createWithFileName(const std::string& fileName);
    };
    
    class DynamicObjectManager : public cocos2d::Ref
    {
    private:
        std::vector<DynamicObject*> dynamicObjects;
    public:
        ~DynamicObjectManager();
        
        static DynamicObjectManager* getInstance();
        static void destroyInstance();
        
        void loadObjects();
        
        size_t getNumObjects() const { dynamicObjects.size(); }
        const std::string& getTextureFilePath(const size_t objectIndex) const { return dynamicObjects[objectIndex]->getTextureFilePath(); };
        DynamicObject* getByFileName(const std::string& fileName) const;
    };
}

#endif /* DynamicObjectManager_hpp */
