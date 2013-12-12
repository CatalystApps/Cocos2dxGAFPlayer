#pragma once

#ifndef __GAF_ANIMATION__
#define __GAF_ANIMATION__

#include "cocoa/CCObject.h"
#include <string>

#define GAF_FIRST_FRAME_INDEX 0

class GAFAsset;
class GAFAnimatable;

namespace cocos2d
{
	class CCDictionary;
}

using namespace cocos2d;

enum AnimSetSequenceHint
{
	ASSH_CONTINUE,
	ASSH_RESTART
};

class GAFAnimatedObject;

class GAFSequenceDelegate
{
public:
	virtual void onFinishSequence(GAFAnimatedObject * object, const std::string& sequenceName) = 0;
};

class GAFAnimation
{
public:
	~GAFAnimation();
	bool init(GAFAsset * anAnimationData);
	
	virtual void processAnimation();
	virtual void start();
	virtual void pause();
	virtual void resume();
	virtual void stop();
	virtual void step();
	
	bool isDone() const;
	bool isAnimationRunning() const;
	bool isLooped() const;
	void setLooped(bool looped);
	int totalFrameCount() const;
	int currentFrameIndex() const;

	bool setFrame(int index);
	
	bool gotoAndStop(const char * frameLabel);
	bool gotoAndStop(int frameNumber);
	
	bool gotoAndPlay(const char * frameLabel);
	bool gotoAndPlay(int frameNumber);
	
	
	int getStartFrame(const char * frameLabel);
	int getEndFrame(const char * frameLabel);
	
	bool playSequence(const char * name, bool looped = false, bool resume = true, AnimSetSequenceHint hint = ASSH_RESTART);
	void clearSequence();
	// do not forget to call setSequenceDelegate(NULL) before deleting your subscriber
	void setSequenceDelegate(GAFSequenceDelegate * delegate);
protected:
	GAFAnimation();
protected:
	GAFAsset * _asset;
	int _currentFrameIndex;
private:
	GAFSequenceDelegate * _sequenceDelegate;
	int _totalFrameCount;
	int _currentSequenceStart;
	int _currentSequenceEnd;
	bool _isRunning;
	bool _isLooped;
};


#endif // __GAF_ANIMATION__
