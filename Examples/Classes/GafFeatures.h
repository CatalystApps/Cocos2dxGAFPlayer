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

    void black(CCObject*);
    void white(CCObject*);
    void gray(CCObject*);
    void addOne(CCObject*);
    void prevFrame(CCObject*);
    void nextFrame(CCObject*);
    void removeOne(CCObject*);
    void set1(CCObject*);
    void set5(CCObject*);
    void set10(CCObject*);
	void set(int n);
    void restart(CCObject*);
    void playpause(CCObject*);
    void cleanup(CCObject*);
    void next_anim(CCObject*);
    void prev_anim(CCObject*);
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
