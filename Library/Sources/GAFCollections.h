#pragma once

#include <unordered_map>
#include "GAFAnimationSequence.h"

NS_GAF_BEGIN

class GAFTextureAtlas;
class GAFAnimationFrame;
class GAFSprite;
class GAFFilterData;

enum GAFFilterType
{
    GFT_DropShadow = 0,
    GFT_Blur = 1,
    GFT_Glow = 2,
    GFT_ColorMatrix = 6
};

typedef std::vector<GAFTextureAtlas*>               TextureAtlases_t;
typedef std::map<unsigned int, unsigned int>        AnimationMasks_t;      // Object id -> Element Atlas Id
typedef std::map<unsigned int, unsigned int>        AnimationObjects_t;    // Object id -> Element Atlas Id
typedef std::vector<GAFAnimationFrame*>             AnimationFrames_t;
//typedef std::unordered_map<unsigned int, GAFSprite*> SubObjects_t;
typedef std::vector<GAFSprite*> SubObjects_t;
typedef std::vector<GAFFilterData*>                 Filters_t;

typedef std::unordered_map<unsigned int, int>       CaptureObjects_t;      // Object id -> capture flags

// TODO: it better to use HashMap(Unordered map)
typedef std::map<std::string, GAFAnimationSequence> AnimationSequences_t;
typedef std::map<std::string, unsigned int>         NamedParts_t;

enum
{
    IDNONE = UINT_MAX
};

NS_GAF_END
