//
//  ResourceManager.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 24.04.16.
//
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include "cocos2d.h"

namespace ottersimulator
{
    class ResourceManager : public cocos2d::Ref
    {
    public:
        enum ResourceType
        {
            kStaticObjects = 0,
            kDynamicObjects = 1,
            kBackgrounds = 2
        };
    private:
        std::vector<std::string> staticObjectFilePathes;
        std::vector<std::string> backgroundFilePathes;
        std::vector<std::string> dynamicObjectFilePathes;
        
        void loadStaticObjects();
        void loadDynamicObjects();
        void loadBackgrounds();
        static void setLocalPathes(const std::vector<std::string>& filePathes, const std::string& folder, std::vector<std::string>& localPathes);
    public:
        ResourceManager();
        virtual ~ResourceManager();
        
        static ResourceManager* getInstance();
        static void destroyInstance();
        
        void loadResources();
        size_t getNumResources(const ResourceType resourceType) const;
        const std::string& getStaticObjectFilePath(const size_t resourceIndex) const { return staticObjectFilePathes[resourceIndex]; }
        const std::string& getBackgroundFilePath(const size_t resourceIndex) const { return backgroundFilePathes[resourceIndex]; }
        const std::string& getDynamicObjectFilePath(const size_t resourceIndex) const { return dynamicObjectFilePathes[resourceIndex]; }
    };
}

#endif /* ResourceManager_hpp */
