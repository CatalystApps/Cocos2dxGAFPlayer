#pragma once

class GAFTextureAtlas;
class GAFAnimationFrame;
class GAFSprite;
class GAFFilterData;
class GAFTimeline;

#include <unordered_map>
#include "GAFAnimationSequence.h"

enum GAFFilterType
{
    GFT_DropShadow = 0,
    GFT_Blur = 1,
    GFT_Glow = 2,
    GFT_ColorMatrix = 6
};

enum GAFCharacterType
{
	GCT_TEXTURE = 0,
	GCT_TEXT_FIELD,
	GCT_TIMELINE
};

typedef std::tuple<unsigned int, GAFCharacterType>	AnimationObjectEx_t;
typedef std::vector<GAFTextureAtlas*>               TextureAtlases_t;
typedef std::map<unsigned int, AnimationObjectEx_t>	AnimationMasks_t;      // Object id -> Element Atlas Id
typedef std::map<unsigned int, AnimationObjectEx_t>	AnimationObjects_t;    // Object id -> Element Atlas Id
typedef std::vector<GAFAnimationFrame*>             AnimationFrames_t;
typedef std::unordered_map<unsigned int, GAFSprite*> SubObjects_t;
typedef std::vector<GAFFilterData*>                 Filters_t;
typedef std::vector<GAFTimeline*>					Timelines_t;

typedef std::unordered_map<unsigned int, int>       CaptureObjects_t;      // Object id -> capture flags

// TODO: it better to use HashMap(Unordered map)
typedef std::map<std::string, GAFAnimationSequence> AnimationSequences_t;
typedef std::map<std::string, unsigned int>         NamedParts_t;

#define GAF_SAFE_RELEASE_MAP(__type, __instance)\
for (__type::iterator i = __instance.begin(), e = __instance.end(); i != e; ++i) {\
i->second->release(); \
} __instance.clear();

#define GAF_SAFE_RELEASE_ARRAY(__type, __instance)\
for (__type::iterator i = __instance.begin(), e = __instance.end(); i != e; ++i) {\
(*i)->release(); \
} __instance.clear();

#define GAF_RELEASE_ARRAY(__type, __instance)\
for (__type::iterator i = __instance.begin(), e = __instance.end(); i != e; ++i) {\
delete *i; \
} __instance.clear();

#define GAF_RELEASE_MAP(__type, __instance)\
for (__type::iterator i = __instance.begin(), e = __instance.end(); i != e; ++i) {\
delete i->second; \
} __instance.clear();


enum
{
    IDNONE = UINT_MAX
};
