#ifndef __GafFeatures_SCENE_H__
#define __GafFeatures_SCENE_H__

#include "cocos2d.h"
#include <string>
#include <vector>

using namespace cocos2d;

class GAFAsset;

class GafFeatures : public cocos2d::CCLayer
{
public:
	GafFeatures();
	~GafFeatures();
	
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GafFeatures);
	
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	
	CCMenuItemImage * addBtn(const char * text, float px, float py, SEL_MenuHandler handler, float k = 1);

	void black();
	void white();
	void gray();
	void addOne();
	void prevFrame();
	void nextFrame();
	void removeOne();
	void set1();
	void set5();
	void set10();
	void set(int n);
	void restart();
	void playpause();
	void cleanup();
	void next_anim();
	void prev_anim();
	int maxFrameNumber();
	void setFrameNumber(int aFrameNumber);
	int frameNumber();
	void addObjectsToScene(int aCount);
	void removeFromScene(int aCount);
	
private:	
	GAFAsset * _asset;
	CCArray * _objects;
	std::vector<std::string>               _jsons;
	//std::vector<std::vector<std::string> > _atlases;
	int _anim_index;

};

#endif // __GafFeatures_SCENE_H__
