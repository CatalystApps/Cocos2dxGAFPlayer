#pragma once

#ifndef __GAF_ANIMATED_OBJECT_H__
#define __GAF_ANIMATED_OBJECT_H__

#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "GAFAnimation.h"
#include <vector>
#include <string>

class GAFAnimation;
class GAFSprite;
class GAFAnimator;
class GAFAnimatedObject;
class GAFAsset;

namespace cocos2d
{
	class CCDictionary;
	class CCArray;
	class CCString;
}

using namespace cocos2d;

enum PCAnimationFPSType
{
    kGAFAnimationFPSType_15 = 15,
    kGAFAnimationFPSType_30 = 30,
    kGAFAnimationFPSType_60 = 60
};

enum GAFAnimatedObjectControlFlags
{
    kGAFAnimatedObjectControl_None = 0,
    // If specified, state of controlled object will be changed every frame (like it is by default) and then
    // animatedObject:didDisplayFrameWithSubobject: will be called
    kGAFAnimatedObjectControl_ApplyState = 1 << 0
};

class GAFFramePlayedDelegate
{
public:
	virtual void onFramePlayed(GAFAnimatedObject * object, int frame);
};

class GAFAnimatedObjectControlDelegate
{
public:
	virtual void onFrameDisplayed(GAFAnimatedObject * object, GAFSprite * subobject);
};

class GAFAnimatedObject : public CCLayer, public GAFAnimation
{
public:
	~GAFAnimatedObject();
	static GAFAnimatedObject * create(GAFAsset * anAsset);
	static GAFAnimatedObject * create(const char * jsonPath);	
	static GAFAnimatedObject * createAndRun(const char * jsonPath, bool looped = false);

	bool init(GAFAsset * anAsset);	
	void processAnimations(float dt);
	CCPoint pupilCoordinatesWithXSemiaxis(float anXSemiaxis, float anYSemiaxis, CCPoint aCenter, CCPoint anExternalPoint);
	GAFSprite * subObjectForInnerObjectId(CCString * anInnerObjectId);
	void removeAllSubObjects();
	void addSubObjectsUsingAnimationObjectsDictionary(CCDictionary * anAnimationObjects, CCDictionary * anAnimationMasks, CCArray * anAnimationFrames);
	void setSubobjectsVisible(bool visible);
	CCDictionary * subObjects();
	CCDictionary * masks();
	void animatorDidPlayedFrame(GAFAnimator * anAnimator, int aFrameNo);
	GAFSprite * subobjectByName(const char * name);
	// call this function only if you really know what you do
	GAFSprite * subobjectByRawName(const char * name);
	virtual void start();
	virtual void stop();
	virtual void processAnimation();
/// Takes control over subobject, which means that every frame control delegate will be notified to decide
/// what to do with captured external object
/// @note it supports only objects for now, DOES NOT SUPPORT MASKS
/// @param subobject name taken from "namedParts" section in config
/// @param controlFlags flags specifying what played will do with subobjects controlled externally
/// @returns YES if control was successfully taken and all future
	bool captureControlOverSubobjectNamed(const char * aName, GAFAnimatedObjectControlFlags aControlFlags);
	void releaseControlOverSubobjectNamed(const char * aName);
	
	void setFramePlayedDelegate(GAFFramePlayedDelegate * delegate);
	void setControlDelegate(GAFAnimatedObjectControlDelegate * delegate);
	CCRect realBoundingBoxForCurrentFrame();
protected:
	GAFAnimatedObject();
	GAFSprite * subObjectForInnerObjectId(const char * anInnerObjectId);
	std::string objectIdByObjectName(const char * aName);

private:
	GAFAsset * _asset;
	CCDictionary   * _subObjects;
	CCDictionary   * _masks;
	CCDictionary   * _capturedObjects;
	bool _animationsSelectorScheduled;
	GAFFramePlayedDelegate * _framePlayedDelegate;
	GAFAnimatedObjectControlDelegate * _controlDelegate;
	
	int numberOfGlobalFramesForOneAnimationFrame();
	int _extraFramesCounter;
	PCAnimationFPSType _FPSType;

}; // GAFAnimatedObject

#endif // __GAF_ANIMATED_OBJECT_H__
