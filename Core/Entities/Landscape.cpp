//
//  Terrain.cpp
//  CoolOtter
//
//  Created by Vladimir Vlasov on 17.04.16.
//
//

#include "global.h"
#include "Map.hpp"
#include "Landscape.hpp"
#include "JSONHelper.hpp"

using namespace cocos2d;
using namespace ottersimulator;

#define BLOCK_KEY_POINTS_COUNT 20
#define KEY_POINT_DISTANCE 1.f
#define DEFAULT_HEIGHT 10
#define CONTOUR_COLOR Color4F(0.f, 0.f, 0.f, 1.f)
#define CONTOUR_THICKNESS .05f
#define INTERPOLATION_STEP .1f

Landscape* Landscape::createFlatBlock()
{
    Landscape *landscape = new (std::nothrow) Landscape();
    if (landscape && landscape->initWithFlatBlock())
    {
        landscape->autorelease();
        return landscape;
    }
    CC_SAFE_DELETE(landscape);
    return nullptr;
}

bool Landscape::init()
{
    if(!Entity::init())
    {
        return false;
    }
    
    typeName = "Landscape";
    
    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
    
    dirtTexture = Director::getInstance()->getTextureCache()->addImage("Textures/Tiles/dirt_0.png");
    dirtTexture->setTexParameters({GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT});
    dirtTexture->retain();
    
    return true;
}

bool Landscape::initWithFlatBlock()
{
    if(!Landscape::init())
    {
        return false;
    }
    
    Block* block = Block::createFlatBlockWithLandscape(this, 0.f, UNITS_IN_SCREEN_HEIGHT * .5f);
    block->retain();
    this->blocks.push_back(block);
    
    this->invalidate();
    
    return true;
}

Landscape::~Landscape()
{
    for(auto block : this->blocks)
    {
        CC_SAFE_RELEASE(block);
    }
    this->blocks.clear();
    
    this->dirtTexture->release();
}

void Landscape::addFlatBlockToTheLeft()
{
    auto firstBlock = this->blocks.empty() ? nullptr : this->blocks.front();
    auto firstPoint = firstBlock ? firstBlock->getFirstControlPoint() : nullptr;
    float height = firstBlock ? firstPoint->getPosition().y : UNITS_IN_SCREEN_HEIGHT * .5f;
    float offset = firstBlock ? (firstPoint->getPosition().x - KEY_POINT_DISTANCE * (BLOCK_KEY_POINTS_COUNT - 1)) : 0.f;
    
    Block* block = Block::createFlatBlockWithLandscape(this, offset, height);
    block->retain();
    this->blocks.insert(this->blocks.begin(), block);
    
    if(firstBlock)
    {
        this->connectBlocks(block, firstBlock);
    }
    
    auto map = static_cast<ottersimulator::Map*>(this->getParent());
    this->setVisibleRect(map->getVisibleRect());
    
    this->invalidate();
}

void Landscape::addFlatBlockToTheRight()
{
    auto lastBlock = this->blocks.empty() ? nullptr : this->blocks.back();
    auto lastPoint = lastBlock ? lastBlock->getLastControlPoint() : nullptr;
    float height = lastPoint ? lastPoint->getPosition().y : UNITS_IN_SCREEN_HEIGHT * .5f;
    float offset = lastPoint ? lastPoint->getPosition().x : 0.f;
    
    Block* block = Block::createFlatBlockWithLandscape(this, offset, height);
    block->retain();
    this->blocks.push_back(block);
    
    if(lastBlock)
    {
        this->connectBlocks(lastBlock, block);
    }
    
    auto map = static_cast<ottersimulator::Map*>(this->getParent());
    this->setVisibleRect(map->getVisibleRect());
    
    this->invalidate();
}

void Landscape::updateBounds()
{
    keyPointBounds.setRect(0, 0, 0, 0);
    
    for(auto block : this->blocks)
    {
        const Rect bounds = block->getBounds();
        
        if(bounds.origin.x < keyPointBounds.origin.x)
        {
            keyPointBounds.setRect(bounds.origin.x, keyPointBounds.origin.y, keyPointBounds.origin.x - bounds.origin.x + keyPointBounds.size.width, keyPointBounds.size.height);
        }
        
        if(bounds.origin.y < keyPointBounds.origin.y)
        {
            keyPointBounds.setRect(keyPointBounds.origin.x, bounds.origin.y,  keyPointBounds.size.width, keyPointBounds.origin.y - bounds.origin.y + keyPointBounds.size.height);
        }
        
        if(bounds.getMaxX() > keyPointBounds.getMaxX())
        {
            keyPointBounds.size = Size(bounds.getMaxX() - keyPointBounds.origin.x, keyPointBounds.size.height);
        }
        
        if(bounds.getMaxY() > keyPointBounds.getMaxY())
        {
            keyPointBounds.size = Size(keyPointBounds.size.width, bounds.getMaxY() - keyPointBounds.origin.y);
        }
    }
}

void Landscape::invalidate()
{
    this->removeAllChildren();
    this->updateBounds();
    this->initControlPointsForEditing();
}

void Landscape::onDrawTerrain()
{
    CC_NODE_DRAW_SETUP();
    
    std::for_each(this->blocks.begin(), this->blocks.end(), [](Block* block)
    {
        block->draw();
    });
}

void Landscape::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    this->drawCommand.init(this->getGlobalZOrder());
    this->drawCommand.func = std::bind(&Landscape::onDrawTerrain, this);
    
    renderer->addCommand(&drawCommand);

    Node::draw(renderer, transform, flags);
}

void Landscape::setVisibleRect(const cocos2d::Rect& rect)
{
    this->visibleBlocks.clear();
    
    for_each(this->blocks.begin(), this->blocks.end(), [this, &rect](Block* block)
             {
                 if(rect.intersectsRect(block->getBounds()))
                 {
                     this->visibleBlocks.push_back(block);
                 }
             });
    
    if(!this->visibleBlocks.empty())
    {
        auto blocksEnd = this->blocks.end();

        auto beforeFirstBlockIt = std::prev(std::find(this->blocks.begin(), this->blocks.end(), this->visibleBlocks.front()));
        if(beforeFirstBlockIt != blocksEnd)
        {
            this->visibleBlocks.push_front(*beforeFirstBlockIt);
        }

        auto afterLastBlockIt = std::next(std::find(this->blocks.begin(), this->blocks.end(), this->visibleBlocks.back()));
        if(afterLastBlockIt != blocksEnd)
        {
            this->visibleBlocks.push_back(*afterLastBlockIt);
        }
    }
    
    for_each(this->blocks.begin(), this->blocks.end(), [this](Block* block)
             {
                 auto blockIt = std::find(this->visibleBlocks.begin(), this->visibleBlocks.end(), block);
                 bool isVisible = (blockIt != this->visibleBlocks.end());
                 block->setVisibility(isVisible);
             });
}

//void Landscape::setControlPointHeight(cp_iterator& controlPoint, float height)
//{
//    
//}

//float Landscape::getHeightAtPoint(float xPos) const
//{
//    if(xPos < keyPointBounds.getMinX() || xPos > keyPointBounds.getMaxX())
//    {
//        return NAN;
//    }
//    
//    size_t iKeyPoint = (size_t)((xPos - keyPointBounds.getMinX()) / KEY_POINT_DISTANCE);
//    if(iKeyPoint < keyPoints.size() - 1)
//    {
//        Point keyPoint0 = keyPoints[iKeyPoint];
//        Point keyPoint1 = keyPoints[iKeyPoint + 1];
//        float t = (xPos - keyPoint0.x) / (keyPoint1.x - keyPoint0.x);
//        return t * keyPoint0.y + (1 - t) * keyPoint1.y;
//    }
//    else
//    {
//        return keyPoints[iKeyPoint].y;
//    }
//}

float Landscape::getHeightAtPoint(float xPos)
{
    if(xPos < keyPointBounds.getMinX() || xPos > keyPointBounds.getMaxX())
    {
        return NAN;
    }
    
    for(auto block : this->blocks)
    {
        Rect bounds = block->getBounds();
        if(xPos >= bounds.getMinX() && xPos <= bounds.getMaxX())
        {
            return block->getHeightAtPoint(xPos);
        }
    }
    
    CC_ASSERT(false);
}

bool Landscape::loadImpl(const rapidjson::Value& json)
{
    const rapidjson::Value& blocksArray = json["blocks"];
    for(rapidjson::SizeType i = 0; i < blocksArray.Size(); i++)
    {
        Block* block = Block::createWithLandscape(this);
        block->retain();
        block->load(blocksArray[i]);
        this->blocks.push_back(block);
        
        if(i > 0)
        {
            this->connectBlocks(this->blocks[i - 1], block);
        }
    }
    
    this->invalidate();
    
    return Entity::loadImpl(json);
}

bool Landscape::saveImpl(rapidjson::Document& document, rapidjson::Value& json)
{
    rapidjson::Value blocksValue(rapidjson::kArrayType);
    for(const auto block : this->blocks)
    {
        rapidjson::Value blockValue(rapidjson::kObjectType);
        block->save(document, blockValue);
        
        blocksValue.PushBack(blockValue, document.GetAllocator());
    }
    json.AddMember("blocks", blocksValue, document.GetAllocator());
    
    return Entity::saveImpl(document, json);
}

void Landscape::connectBlocks(Block* left, Block* right)
{
    auto previousLastPoint = left->getLastControlPoint();
    auto firstPoint = right->getFirstControlPoint();
    previousLastPoint->setDependentPoint(firstPoint);
    firstPoint->setHeadingPoint(previousLastPoint);
}

void Landscape::initControlPointsForEditing()
{
    this->keyPointsForEditing.clear();
    this->keyPointsForEditing.reserve(this->blocks.size() * BLOCK_KEY_POINTS_COUNT);
    
    for_each(this->blocks.begin(), this->blocks.end(), [this](Block* block)
             {
                 size_t nPoints = (block == this->blocks.back()) ? block->getControlPointCount() : block->getControlPointCount() - 1;
                 
                 for(size_t iPoint = 0; iPoint < nPoints; iPoint++)
                 {
                     this->keyPointsForEditing.push_back(block->getControlPoint(iPoint));
                 }
             });
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ControlPoint* ControlPoint::createWithPositionAndBlock(const cocos2d::Vec2& position, Block* block)
{
    ControlPoint *controlPoint = new (std::nothrow) ControlPoint();
    if (controlPoint && controlPoint->initWithPositionAndBlock(position, block))
    {
        controlPoint->autorelease();
        return controlPoint;
    }
    CC_SAFE_DELETE(controlPoint);
    return nullptr;
}

bool ControlPoint::initWithPositionAndBlock(const cocos2d::Vec2& position, Block* block)
{
    this->position = position;
    this->block = block;
    
    return true;
}

void ControlPoint::setHeight(float height)
{
    this->position.y = height;
    
    if(this->dependentPoint)
    {
        this->dependentPoint->position.y = height;
        this->dependentPoint->getBlock()->invalidate();
    }
    
    if(this->headingPoint)
    {
        this->headingPoint->position.y = height;
        this->headingPoint->getBlock()->invalidate();
    }
    
    this->block->invalidate();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Block::Block(Landscape* landscape) : landscape(landscape)
//{
//    this->vertexBuffer = NULL;
//    this->textureCoordsBuffer = NULL;
//    this->contourVerticesBuffer = NULL;
//    this->contourColorBuffer = NULL;
//}
//
//Block::~Block()
//{
//    for(auto point : this->keyPoints)
//    {
//        delete point;
//    }
//}

Block::~Block()
{
    for(auto point : this->keyPoints)
    {
        CC_SAFE_RELEASE(point);
    }
    this->keyPoints.clear();
}

Block* Block::createWithLandscape(Landscape* landscape)
{
    Block* block = new (std::nothrow) Block();
    if (block && block->initWithLandscape(landscape))
    {
        block->autorelease();
        return block;
    }
    CC_SAFE_DELETE(block);
    return nullptr;
}

Block* Block::createFlatBlockWithLandscape(Landscape* landscape, float offset, float height)
{
    Block* block = new (std::nothrow) Block();
    if (block && block->initFlatBlockWithLandscape(landscape, offset, height))
    {
        block->autorelease();
        return block;
    }
    CC_SAFE_DELETE(block);
    return nullptr;
}

bool Block::initWithLandscape(Landscape* landscape)
{
    this->landscape = landscape;
    
    this->nVertices = 0;
    this->vertexBuffer = NULL;
    this->textureCoordsBuffer = NULL;
    this->contourVerticesBuffer = NULL;
    this->contourColorBuffer = NULL;
    
    return true;
}

bool Block::initFlatBlockWithLandscape(Landscape* landscape, float offset, float height)
{
    if(!this->initWithLandscape(landscape))
    {
        return false;
    }
    
    this->keyPoints.reserve(BLOCK_KEY_POINTS_COUNT);
    for(size_t iKeyPoint = 0; iKeyPoint < BLOCK_KEY_POINTS_COUNT; iKeyPoint++)
    {
        Vec2 position = Vec2(offset + KEY_POINT_DISTANCE * iKeyPoint, height);
        auto point = ControlPoint::createWithPositionAndBlock(position, this);
        point->retain();
        this->keyPoints.push_back(point);
    }
    
    this->invalidate();
    
    return true;
}

void Block::draw()
{
    GL::bindTexture2D(this->landscape->getDirtTexture()->getName());
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, this->vertexBuffer);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, this->textureCoordsBuffer);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->nVertices);
    
    //    CC_NODE_DRAW_SETUP();
    
    GL::bindTexture2D(0);
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, this->contourVerticesBuffer);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, this->contourColorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->nVertices);
}

float Block::getHeightAtPoint(float xPos)
{
    if(xPos < keyPointBounds.getMinX() || xPos > keyPointBounds.getMaxX())
    {
        return NAN;
    }
    
    size_t iKeyPoint = (size_t)((xPos - keyPointBounds.getMinX()) / KEY_POINT_DISTANCE);
    if(iKeyPoint < keyPoints.size() - 1)
    {
        Point keyPoint0 = keyPoints[iKeyPoint]->getPosition();
        Point keyPoint1 = keyPoints[iKeyPoint + 1]->getPosition();
        
        float step = 0;
        float minDifference = xPos - keyPoint0.x;
        float yPos = keyPoint0.y;
        
        while(keyPoint0.x + step * (keyPoint1.x - keyPoint0.x) < keyPoint1.x)
        {
            Vec2 point = this->spline.Eval(iKeyPoint, step);
            float xDifference = fabs(point.x - xPos);
            if(xDifference <= minDifference)
            {
                minDifference = xDifference;
                yPos = point.y;
            }
            else
            {
                break;
            }
            
            step += .01f;///!!!magic const
        }
        
        return yPos;
    }
    else
    {
        return keyPoints[iKeyPoint]->getPosition().y;
    }
}


void Block::updateBounds()
{
    keyPointBounds.setRect(0, 0, 0, 0);
    
    if(keyPoints.size())
    {
        float minY = 0;
        float maxY = 0;
        std::for_each(keyPoints.begin(), keyPoints.end(), [&minY, &maxY](ControlPoint* keyPoint)
        {
            if(keyPoint->getPosition().y < minY)
            {
              minY = keyPoint->getPosition().y;
            }
            if(keyPoint->getPosition().y > maxY)
            {
              maxY = keyPoint->getPosition().y;
            }
        });
        
        float width = keyPoints.back()->getPosition().x - keyPoints.front()->getPosition().x;
        float height = maxY - minY + DEFAULT_HEIGHT;
        keyPointBounds.setRect(keyPoints.front()->getPosition().x, minY - DEFAULT_HEIGHT, width, height);
    }
}

bool Block::load(const rapidjson::Value& json)
{
    const rapidjson::Value& keyPointsArray = json["keypoints"];
    this->keyPoints.reserve(keyPointsArray.Size());
    for(rapidjson::SizeType i = 0; i < keyPointsArray.Size(); i++)
    {
        auto point = ControlPoint::createWithPositionAndBlock(ParseVec2(keyPointsArray[i]), this);
        point->retain();
        this->keyPoints.push_back(point);
    }
    
    this->invalidate();
    
    return true;
}

bool Block::save(rapidjson::Document& document, rapidjson::Value& json) const
{
    rapidjson::Value keyPointsValue(rapidjson::kArrayType);
    for(const auto& keyPoint : this->keyPoints)
    {
        rapidjson::Value keyPointValue(rapidjson::kArrayType);
        keyPointValue.PushBack(rapidjson::Value().SetDouble(keyPoint->getPosition().x), document.GetAllocator());
        keyPointValue.PushBack(rapidjson::Value().SetDouble(keyPoint->getPosition().y), document.GetAllocator());
        
        keyPointsValue.PushBack(keyPointValue, document.GetAllocator());
    }
    json.AddMember("keypoints", keyPointsValue, document.GetAllocator());
    
    return true;
}

void Block::invalidate()
{
    this->spline.Reset();
    this->cleanGeometry();
    
    this->updateBounds();
    
    if(keyPoints.size())
    {
        spline.SetElimColinearPoints(false);
        for(auto point : keyPoints)
        {
            spline.AddPoint(point->getPosition());
        }
        
        spline.ComputeSpline();
    }
    
    if(this->visible)
    {
        this->updateGeometry();
    }
}

void Block::updateGeometry()
{
    if(!this->needUpdateGeometry)
    {
        return;
    }
    
    if(keyPoints.size())
    {
        this->nVertices = (keyPoints.size() - 1) * (KEY_POINT_DISTANCE / INTERPOLATION_STEP) * 6;
        
        this->vertexBuffer = (Vec2*)malloc(sizeof(Vec2) * this->nVertices);
        this->textureCoordsBuffer = (Vec2*)malloc(sizeof(Vec2) * this->nVertices);
        this->contourVerticesBuffer = (Vec2*)malloc(sizeof(Vec2) * this->nVertices);
        this->contourColorBuffer = (Color4F*)malloc(sizeof(Color4F) * this->nVertices);
        
        size_t iVertex = 0;
        size_t iContourVertex = 0;
        
        for(size_t iKeyPoint = 0; iKeyPoint < keyPoints.size() - 1; iKeyPoint++)
        {
            Vec2 baseKeyPoint = keyPoints[iKeyPoint]->getPosition();
            
            for(float step = 0; step < 1; step += INTERPOLATION_STEP)
            {
                Vec2 p0 =  spline.Eval(iKeyPoint, step / KEY_POINT_DISTANCE);
                
                float stepEnd = (step + INTERPOLATION_STEP) > 1 ? 1.f : step + INTERPOLATION_STEP;
                Vec2 p1 =  spline.Eval(iKeyPoint, stepEnd / KEY_POINT_DISTANCE);
                
                //Terrain
                
                Vec2 v0 = Vec2(p0.x, keyPointBounds.getMinY());
                Vec2 v1 = Vec2(p0.x, p0.y - CONTOUR_THICKNESS);
                Vec2 v2 = Vec2(p1.x, keyPointBounds.getMinY());
                Vec2 v3 = Vec2(p1.x, p1.y - CONTOUR_THICKNESS);
                
                //!!!bounds with ratio
                float xTextureCoordStart = (iKeyPoint + (p0.x - baseKeyPoint.x) / KEY_POINT_DISTANCE) * 10.f / (keyPoints.size() - 1);
                float xTextureCoordEnd = (iKeyPoint + (p1.x - baseKeyPoint.x) / KEY_POINT_DISTANCE) * 10.f / (keyPoints.size() - 1);
                
                Vec2 t0 = {xTextureCoordStart, 0};
                Vec2 t1 = {xTextureCoordStart, 10};
                Vec2 t2 = {xTextureCoordEnd, 0};
                Vec2 t3 = {xTextureCoordEnd, 10};
                
                vertexBuffer[iVertex] = v0;
                textureCoordsBuffer[iVertex++] = t0;
                
                vertexBuffer[iVertex] = v1;
                textureCoordsBuffer[iVertex++] = t1;
                
                vertexBuffer[iVertex] = v2;
                textureCoordsBuffer[iVertex++] = t2;
                
                vertexBuffer[iVertex] = v2;
                textureCoordsBuffer[iVertex++] = t2;
                
                vertexBuffer[iVertex] = v1;
                textureCoordsBuffer[iVertex++] = t1;
                
                vertexBuffer[iVertex] = v3;
                textureCoordsBuffer[iVertex++] = t3;
                
                //Contour
                
                //!!!use tangents
                
                Vec2 cv0 = Vec2(p0.x, p0.y - CONTOUR_THICKNESS);
                Vec2 cv1 = p0;
                Vec2 cv2 = Vec2(p1.x, p1.y - CONTOUR_THICKNESS);
                Vec2 cv3 = p1;
                
                contourVerticesBuffer[iContourVertex] = cv0;
                contourColorBuffer[iContourVertex++] = CONTOUR_COLOR;
                
                contourVerticesBuffer[iContourVertex] = cv1;
                contourColorBuffer[iContourVertex++] = CONTOUR_COLOR;
                
                contourVerticesBuffer[iContourVertex] = cv2;
                contourColorBuffer[iContourVertex++] = CONTOUR_COLOR;
                
                contourVerticesBuffer[iContourVertex] = cv2;
                contourColorBuffer[iContourVertex++] = CONTOUR_COLOR;
                
                contourVerticesBuffer[iContourVertex] = cv1;
                contourColorBuffer[iContourVertex++] = CONTOUR_COLOR;
                
                contourVerticesBuffer[iContourVertex] = cv3;
                contourColorBuffer[iContourVertex++] = CONTOUR_COLOR;
            }
        }
        
        CC_ASSERT(iVertex == this->nVertices);
        CC_ASSERT(iContourVertex == this->nVertices);
    }
    
    this->needUpdateGeometry = false;
}

void Block::cleanGeometry()
{
    CC_SAFE_FREE(this->vertexBuffer);
    CC_SAFE_FREE(this->textureCoordsBuffer);
    CC_SAFE_FREE(this->contourVerticesBuffer);
    CC_SAFE_FREE(this->contourColorBuffer);
    
    nVertices = 0;
    
    needUpdateGeometry = true;
}

void Block::setVisibility(bool isVisible)
{
    if(this->visible == isVisible)
    {
        return;
    }
    
    this->visible = isVisible;
    
    if(isVisible)
    {
        updateGeometry();
    }
    else
    {
        cleanGeometry();
    }
}

//void Landscape::invalidate()
//{
//    stripes.clear();
//    removeAllChildren();
//
//    std::vector<Vec2> vertices;
//    std::vector<ccColor4F> colors;
//
//    for(size_t iKeyPoint = 1; iKeyPoint < keyPoints.size(); iKeyPoint++)
//    {
//        Point start = keyPoints[iKeyPoint - 1];
//        Point end = keyPoints[iKeyPoint];
//
//        auto stripe = DrawNode::create();
//        stripe->setLineWidth(STRIPE_LINE_WIDTH);
//        stripe->drawLine(keyPoints[iKeyPoint - 1], keyPoints[iKeyPoint], STRIPE_COLOR);
//
//        stripes.push_back(stripe);
//        //addChild(stripe);
//
//        if(iKeyPoint == 1)
//        {
//            vertices.push_back(start + Vec2(0, -DEFAULT_HEIGHT));
//            colors.push_back({0, 1, 0, 1});
//            vertices.push_back(start);
//            colors.push_back({0, 1, 0, 1});
//        }
//
//        vertices.push_back(end + Vec2(0, -DEFAULT_HEIGHT));
//        colors.push_back({0, 1, 0, 1});
//        vertices.push_back(end);
//        colors.push_back({0, 1, 0, 1});
//    }
//
//    auto renderingTexture = RenderTexture::create(512, 512);
//    renderingTexture->beginWithClear(1, 1, 1, 1);
//
//    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
//
//    CC_NODE_DRAW_SETUP();
//
//    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
//
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, &vertices.front());
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, &colors.front());
//    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
//
//    //    auto dirt = Sprite::create("Textures/Tiles/dirt_0.png");
//    ////    dirt->setScale(2);
//    //    dirt->setPosition(100, 100);
//    //    dirt->setBlendFunc({GL_DST_COLOR, GL_ZERO});
//    //    dirt->visit();
//
//    renderingTexture->end();
//
//    addChild(renderingTexture, -1);
//}