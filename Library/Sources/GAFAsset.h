#pragma once

#ifndef __GAF_ASSET_H__
#define __GAF_ASSET_H__

#include "cocoa/CCObject.h"
#include <string>
#include <vector>

namespace cocos2d
{
	class CCDictionary;
	class CCArray;
}
using namespace cocos2d;

class GAFTextureAtlas;
class GAFAnimatedObject;
class GAFAnimationSequence;

class GAFAsset : public CCObject
{
public :	
	static GAFAsset * create(const std::string& jsonPath);
	bool  initWithImageData(const std::string& jsonPath);
	GAFAsset();
	~GAFAsset();
	int animationFramesCount() const;
	GAFTextureAtlas * textureAtlas();
	CCDictionary   * objects();
	CCDictionary   * masks();
	CCDictionary   * namedParts();
	CCDictionary   * animationSequences();
	GAFAnimationSequence   * getSequence(const char * name);
	GAFAnimationSequence   * getSequenceByLastFrame(int frame);
	CCArray        * animationFrames();
	static bool isAssetVersionPlayable(const char * version);
	
	GAFAnimatedObject * createObject();
	GAFAnimatedObject * createObjectAndRun(bool looped = false);
	
	static int desiredCsf();
	static void setDesiredCsf(int csf);
	inline float usedAtlasContentScaleFactor()const
	{
		return _usedAtlasContentScaleFactor;
	}
private:
	void loadAnimationSequences(CCArray * aSequencesNodes);
	void loadFramesFromConfigDictionary(CCDictionary * aConfigDictionary);
	CCArray * objectStatesFromConfigFrame(CCDictionary * configFrame);
	
private:
	float _usedAtlasContentScaleFactor;
	float atlasScaleFromAtlasConfig(CCDictionary * anAtlasConfigDictionary);
	int _majorVersion;
	int _minorVersion;
	GAFTextureAtlas * _textureAtlas;
// dictionary of objects [ObjectId -> AtlasElementName]	
	CCDictionary   * _objects;
// dictionary of masks [MaskId -> AtlasElementName]	
	CCDictionary   * _masks;
/// List of GAFActionObject-s representing zones to interact with	
	CCArray        * _interactionObjects;
/// List of GAFActionObject-s representing zones on which objects can be put
	CCArray        * _standObjects;
	
///< List of GAFAnimationFrame objects
	CCArray        * _animationFrames;
	CCDictionary   * _animationSequences;
	
	CCDictionary   * _namedParts;
};


#endif // __GAF_ASSET_H__
