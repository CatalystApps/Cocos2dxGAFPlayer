#pragma once

#ifndef __GAF_TEXTURE_ATLAS_ELEMENT__
#define __GAF_TEXTURE_ATLAS_ELEMENT__

class GAFTextureAtlasElement
{
public:
    std::string         name;
    cocos2d::CCPoint    pivotPoint;
    cocos2d::CCRect     bounds;
    float               scale;
    unsigned int        atlasIdx;

    unsigned int        elementAtlasIdx;

    bool                init();

    GAFTextureAtlasElement();
}; // GAFTextureAtlasElement

#endif // __GAF_TEXTURE_ATLAS_ELEMENT__
