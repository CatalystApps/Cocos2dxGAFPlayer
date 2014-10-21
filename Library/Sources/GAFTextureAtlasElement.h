#pragma once

NS_GAF_BEGIN

class GAFTextureAtlasElement
{
public:
    std::string name;
    cocos2d::Vect      pivotPoint;
    cocos2d::Rect      bounds;
    float              scale;
    unsigned int       atlasIdx;

    unsigned int       elementAtlasIdx;

    bool               init();

    GAFTextureAtlasElement();
}; // GAFTextureAtlasElement

NS_GAF_END