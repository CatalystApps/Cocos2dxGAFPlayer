#pragma once

#ifndef __GAF_TEXTURE_ATLAS_ELEMENT__
#define __GAF_TEXTURE_ATLAS_ELEMENT__

namespace cocos2d
{
    class CCDictionary;
}

using namespace cocos2d;

class GAFTextureAtlasElement
{
public:
    std::string name;
    CCPoint     pivotPoint;
    CCRect      bounds;
    float       scale;
    unsigned int atlasIdx;

    unsigned int elementAtlasIdx;

    bool                             init();

    GAFTextureAtlasElement();
}; // GAFTextureAtlasElement

#endif // __GAF_TEXTURE_ATLAS_ELEMENT__
