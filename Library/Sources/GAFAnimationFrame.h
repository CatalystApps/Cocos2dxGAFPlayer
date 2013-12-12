#pragma once

#ifndef __GAF_ANIMATION_FRAME__
#define __GAF_ANIMATION_FRAME__

#include "cocoa/CCObject.h"

namespace cocos2d
{
	class CCDictionary;
	class CCArray;
}

class GAFTextureAtlas;

using namespace cocos2d;

class GAFAnimationFrame : public CCObject
{
public:		
	GAFAnimationFrame();
	~GAFAnimationFrame();
	//static GAFAnimationFrame * create(CCDictionary * aDictionary, GAFTextureAtlas * aTextureAtlas);
	//bool init(CCDictionary * aDictionary, GAFTextureAtlas * aTextureAtlas);
	CCArray * objectStates();
	void setObjectStates(CCArray * states);
private:
	CCArray * _initialInstructions;
	CCArray * _objectsStates;
};

#endif // __GAF_ANIMATION_FRAME__
