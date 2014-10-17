#pragma once

#ifndef COCOS2D_VERSION
#error Header "cocos2d.h" must be included
#elif COCOS2D_VERSION < 0x00030101
#error Current version of Cocos2d-x is lower than minimal required v3.1.1\
    For lower versions please use branch `release` from here https://github.com/CatalystApps/Cocos2dxGAFPlayer
#else

#include "GAFAnimatedObject.h"
#include "GAFAnimation.h"
#include "GAFAnimationFrame.h"
#include "GAFAnimationSequence.h"
#include "GAFAsset.h"
#include "GAFCachedTexture.h"
#include "GAFCollections.h"
#include "GAFDelegates.h"
#include "GAFSpriteWithAlpha.h"
#include "GAFStencilMaskSprite.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"

#define GAF_VERSION 3.0

#endif