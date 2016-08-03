//
//  Terrain.hpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#ifndef Terrain_hpp
#define Terrain_hpp

#include "Entity.hpp"
#include "Spline.hpp"

namespace ottersimulator
{
    class Landscape;
    class Block;
    
    class ControlPoint : public cocos2d::Ref
    {
    private:
        Vec2 position;
        Block* block;
        ControlPoint* headingPoint;
        ControlPoint* dependentPoint;
    protected:
        Block* getBlock() const { return block; }
    public:
//        ControlPoint(const cocos2d::Vec2& position, Block* block) : position(position), block(block) {}
        //            ControlPoint(const cocos2d::Vec2& position, Block* block, ControlPoint* headingPoint, ControlPoint* dependentPoint) : position(position), block(block), headingPoint(headingPoint), dependentPoint(dependentPoint) {}
        bool initWithPositionAndBlock(const cocos2d::Vec2& position, Block* block);
        
        Vec2 getPosition() const { return position; }
        void setHeight(float height);
        
        ControlPoint* getHeadingPoint() const { return headingPoint; }
        void setHeadingPoint(ControlPoint* point) { headingPoint = point; }
        
        ControlPoint* getDependentPoint() const { return dependentPoint; }
        void setDependentPoint(ControlPoint* point) { dependentPoint = point; }
        
        static ControlPoint* createWithPositionAndBlock(const cocos2d::Vec2& position, Block* block);
    };
    
    typedef std::vector<ControlPoint*>::iterator cp_iterator;
    typedef std::vector<ControlPoint*>::const_iterator cp_const_iterator;
    
    class Block : public cocos2d::Ref
    {
    private:
        Landscape* landscape;
        std::vector<ControlPoint*> keyPoints;
        ClassicSpline spline;
        cocos2d::Rect keyPointBounds;
        size_t nVertices;
        cocos2d::Vec2* vertexBuffer;
        cocos2d::Vec2* textureCoordsBuffer;
        cocos2d::Vec2* contourVerticesBuffer;
        cocos2d::Color4F* contourColorBuffer;
        bool needUpdateGeometry;
        bool visible;
    protected:
        void updateGeometry();
        void cleanGeometry();
    public:
//        Block(Landscape* landscape);
        ~Block();
        
        bool initWithLandscape(Landscape* landscape);
        bool initFlatBlockWithLandscape(Landscape* landscape, float offset, float height);
        
        cp_iterator getControlPointsBegin() { return keyPoints.begin(); }
        cp_iterator getControlPointsEnd() { return keyPoints.end(); }
        ControlPoint* getFirstControlPoint() const { return keyPoints.front(); }
        ControlPoint* getLastControlPoint() const { return keyPoints.back(); }
        ControlPoint* getControlPoint(size_t index) const { return keyPoints[index]; }
        size_t getControlPointCount() const { return keyPoints.size(); }
        float getHeightAtPoint(float xPos);
        const cocos2d::Rect& getBounds() const { return keyPointBounds; }
        Landscape* getLandscape() const { return landscape; }
        
        bool isVisible() const { return visible; }
        void setVisibility(bool isVisible);
        
        void invalidate();
        void updateBounds();
        
        void draw();
        
        bool load(const rapidjson::Value& json);
        bool save(rapidjson::Document& document, rapidjson::Value& json) const;
        
        static Block* createWithLandscape(Landscape* landscape);
        static Block* createFlatBlockWithLandscape(Landscape* landscape, float offset, float height);
    };
    
    class Landscape : public Entity
    {
    private:
        std::vector<Block*> blocks;
        std::list<Block*> visibleBlocks;
        std::vector<ControlPoint*> keyPointsForEditing;
        cocos2d::Texture2D* dirtTexture;
        cocos2d::Rect keyPointBounds;
        cocos2d::CustomCommand drawCommand;
    protected:
        virtual void updateBounds() override;
        void onDrawTerrain();
        virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
        
        virtual bool loadImpl(const rapidjson::Value& json) override;
        virtual bool saveImpl(rapidjson::Document& document, rapidjson::Value& json) override;
        
        void connectBlocks(Block* left, Block* right);
        void initControlPointsForEditing();
    public:
        ~Landscape();
        virtual bool init() override;
        virtual bool initWithFlatBlock();
        void invalidate();
        
        void setVisibleRect(const cocos2d::Rect& rect);
        
        cp_iterator getControlPointsForEditingBegin() { return keyPointsForEditing.begin(); }
        cp_iterator getControlPointsForEditingEnd() { return keyPointsForEditing.end(); }
        size_t getControlPointForEditingCount() const { return keyPointsForEditing.size(); }
        
        cocos2d::Texture2D* getDirtTexture() const { return dirtTexture; }
        
        virtual const cocos2d::Rect& getBounds() const override { return keyPointBounds; }
        
        float getHeightAtPoint(float xPos);
        
        void addFlatBlockToTheLeft();
        void addFlatBlockToTheRight();
        
        CREATE_FUNC(Landscape);
        static Landscape* createFlatBlock();
    };
}

#endif /* Terrain_hpp */
