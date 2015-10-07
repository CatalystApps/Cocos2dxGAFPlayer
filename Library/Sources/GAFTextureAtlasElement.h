#pragma once

#include "GAFCollections.h"

NS_GAF_BEGIN

class GAFTextureAtlasElement
{
public:
    std::string name;
    cocos2d::Vec2      pivotPoint;
    cocos2d::Rect      bounds;
    
    unsigned int       atlasIdx;
    unsigned int       elementAtlasIdx;

    GAFRotation        rotation;

    GAFTextureAtlasElement();  

    void setScale(float s);
    const float getScale() const { return scale; }
    void setScaleX(float s);
    const float getScaleX() const { return scaleX; }
    void setScaleY(float s);
    const float getScaleY() const { return scaleY; }

private:
    float              scale;
    float              scaleX;
    float              scaleY;
}; // GAFTextureAtlasElement

NS_GAF_END