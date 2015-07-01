#pragma once

NS_GAF_BEGIN

class GAFTextureAtlasElement
{
public:
    std::string name;
    cocos2d::Vect      pivotPoint;
    cocos2d::Rect      bounds;
    
    unsigned int       atlasIdx;
    unsigned int       elementAtlasIdx;

    uint8_t            rotation; // -1 90 CCW, 0 - no rotation, 1 - 90 CW

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