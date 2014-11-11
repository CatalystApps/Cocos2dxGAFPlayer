#pragma once

#include <unordered_map>
#include "GAFAnimationSequence.h"

NS_GAF_BEGIN

class GAFTextureAtlas;
class GAFAnimationFrame;
class GAFSprite;
class GAFFilterData;
class GAFTimeline;
class GAFTextData;
class GAFObject;

enum class GAFFilterType : uint32_t
{
    GFT_DropShadow = 0,
    GFT_Blur = 1,
    GFT_Glow = 2,
    GFT_ColorMatrix = 6
};

enum class GAFCharacterType : uint32_t
{
    Texture = 0,
    TextField,
    Timeline
};

enum class GAFObjectType : uint32_t
{
    None = 0,
    MovieClip,
    Mask
};

enum class GAFActionType : int32_t
{
    //0 - stop, 1 - play, 2 - gotoAndStop, 3 - gotoAndPlay, 4 - dispatchEvent
    None = -1,
    Stop = 0,
    Play,
    GotoAndStop,
    GotoAndPlay,
    DispatchEvent,
};

enum
{
    GAFFirstFrameIndex = 0
};

typedef std::tuple<uint32_t, GAFCharacterType>              AnimationObjectEx_t;
typedef std::vector<GAFTextureAtlas*>                       TextureAtlases_t;
typedef std::unordered_map<uint32_t, AnimationObjectEx_t>   AnimationMasks_t;      // Object id -> Element Atlas Id, Type
typedef std::unordered_map<uint32_t, AnimationObjectEx_t>   AnimationObjects_t;    // Object id -> Element Atlas Id, Type
typedef std::vector<GAFAnimationFrame*>                     AnimationFrames_t;

typedef std::vector<GAFFilterData*>                         Filters_t;
typedef std::unordered_map<uint32_t, GAFTimeline*>          Timelines_t;
typedef std::unordered_map<uint32_t, GAFTextData*>          TextsData_t;

typedef std::unordered_map<uint32_t, int>                   CaptureObjects_t;      // Object id -> capture flags

typedef std::unordered_map<std::string, GAFAnimationSequence>         AnimationSequences_t;
typedef std::unordered_map<std::string, uint32_t>                     NamedParts_t;



static const uint32_t IDNONE = UINT_MAX;

NS_GAF_END