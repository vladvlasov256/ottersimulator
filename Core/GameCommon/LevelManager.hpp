//
//  LevelManager.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#ifndef LevelManager_hpp
#define LevelManager_hpp

#include "cocos2d.h"

namespace ottersimulator
{
    class LevelManager : public cocos2d::Ref
    {
    private:
        std::vector<std::string> levelFilePathes;
    public:
        LevelManager();
        virtual ~LevelManager();
        
        static LevelManager* getInstance();
        static void destroyInstance();
        
        void loadLevels();
        size_t getNumLevels();
        const std::string& getLevelFilePath(size_t level) const;
        const std::string& getNewLevelFilePath(size_t& levelIndex);
    };
}

#endif /* LevelManager_hpp */
