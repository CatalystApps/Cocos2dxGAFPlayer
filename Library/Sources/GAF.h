#pragma once

#ifndef COCOS2D_VERSION
#error Header "cocos2d.h" must be included
#elif COCOS2D_VERSION < 0x00030300
#error Current version of Cocos2d-x is lower than minimal required v3.3.0\
    For lower versions please use branch `release` from here https://github.com/CatalystApps/Cocos2dxGAFPlayer
#else

#include "GAFMacros.h"
#include "GAFAsset.h"
#include "GAFTextureAtlas.h"
#include "GAFObject.h"
#include "GAFAssetTextureManager.h"
#include "GAFDelegates.h"
#include "GAFTimeline.h"

#define GAF_VERSION 5.0

#endif
