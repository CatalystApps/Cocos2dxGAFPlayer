#ifndef __GAF_API_H__
#define __GAF_API_H__

#include "cocos2d.h"
#include "GAFAnimation.h"
#include <string>
#include <vector>

using namespace cocos2d;

class GAFAsset;
class GAFAnimatedObject;

class GafApi : public cocos2d::CCLayer, public GAFSequenceDelegate
{
public:
	GafApi();
	~GafApi();
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GafApi);
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	CCMenuItemImage * addBtn(const char * text, float px, float py, SEL_MenuHandler handler, float k = 1);
	void black();
	void white();
	void gray();
	void demo();
	void addToScene(GAFAnimatedObject * object, float px, float py, const char * sequenceName = 0);
	void onFinishSequence(GAFAnimatedObject * object, const std::string& sequenceName);
private:
	GAFAsset * _asset;


};

#endif // __GAF_API_H__
