#include "GafFeatures.h"
#include "SimpleAudioEngine.h"
#include "GAFAnimatedObject.h"
#include "GAFAsset.h"

using namespace cocos2d;
using namespace CocosDenshion;

GafFeatures::GafFeatures()
:
_asset(NULL),
_objects(NULL)
{
}
GafFeatures::~GafFeatures()
{
	CC_SAFE_RELEASE(_asset);
	CC_SAFE_RELEASE(_objects);
}

CCScene* GafFeatures::scene()
{
    CCScene *scene = CCScene::create();
    GafFeatures *layer = GafFeatures::create();
    scene->addChild(layer);
    return scene;
}

CCMenuItemImage * GafFeatures::addBtn(const char * text, float px, float py, SEL_MenuHandler handler, float k)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	CCMenuItemImage *res = CCMenuItemImage::create(
														  "CloseNormal.png",
														  "CloseSelected.png",
														  this,
														  handler );
    res->setPosition( ccp(size.width * px, size.height * py) );
	res->setScale(k);
	
	CCLabelTTF* pLabel = CCLabelTTF::create(text, "Thonburi", 34);
	pLabel->setColor(ccc3(0, 0, 255));
	
	pLabel->setAnchorPoint(CCPoint(1, 0.5));
	pLabel->setScale(k);
    pLabel->setPosition( ccp(res->getPositionX() - res->getContentSize().width * k * 0.5, res->getPositionY()) );
    addChild(pLabel, 100000);
	return res;
}

void GafFeatures::black()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GafFeatures::white()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GafFeatures::gray()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void GafFeatures::prevFrame()
{
	int f = frameNumber();
	if (-1 == f)
	{
		return;
	}
	if (f)
	{
		setFrameNumber(f - 1);
	}
}

void GafFeatures::nextFrame()
{
	int f = frameNumber();
	if (-1 == f)
	{
		return;
	}
	if (f != maxFrameNumber())
	{
		setFrameNumber(f + 1);
	}
}

bool GafFeatures::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	GAFAsset::setDesiredCsf(CCDirector::sharedDirector()->getContentScaleFactor());
#endif
	CCSize size = CCDirector::sharedDirector()->getWinSizeInPixels();
	
	float s = size.height / 640;
	if (s > 1) s = 1;
	float dp = 0.13;
	
	CCArray * items = CCArray::create();
	items->addObject(addBtn("Play/Pause", 0.95, 0.95, menu_selector(GafFeatures::playpause), s));
	items->addObject(addBtn("Restart", 0.95, 0.95 -dp, menu_selector(GafFeatures::restart), s));
	items->addObject(addBtn("B", 0.75, 0.95 - dp* 2, menu_selector(GafFeatures::black), s));
	items->addObject(addBtn("W", 0.85, 0.95 - dp* 2, menu_selector(GafFeatures::white), s));
	items->addObject(addBtn("G", 0.95, 0.95 - dp * 2, menu_selector(GafFeatures::gray), s));
	items->addObject(addBtn("-", 0.85, 0.95 - dp * 3, menu_selector(GafFeatures::removeOne), s));
	items->addObject(addBtn("+", 0.95, 0.95 - dp* 3, menu_selector(GafFeatures::addOne), s));
	items->addObject(addBtn("1", 0.75, 0.95 - dp * 4, menu_selector(GafFeatures::set1), s));
	items->addObject(addBtn("5", 0.85, 0.95 - dp* 4, menu_selector(GafFeatures::set5), s));
	items->addObject(addBtn("10", 0.95, 0.95 - dp * 4, menu_selector(GafFeatures::set10), s));
	items->addObject(addBtn("-", 0.85, 0.95 - dp * 5, menu_selector(GafFeatures::prevFrame), s));
	items->addObject(addBtn("+", 0.95, 0.95 - dp* 5, menu_selector(GafFeatures::nextFrame), s));
	items->addObject(addBtn("Full cleanup", 0.95, 0.95 - dp* 6, menu_selector(GafFeatures::cleanup), s));
	items->addObject(addBtn("-", 0.85, 0.95 - dp * 7, menu_selector(GafFeatures::prev_anim), s));
	items->addObject(addBtn("+", 0.95, 0.95 - dp* 7, menu_selector(GafFeatures::next_anim), s));

    CCMenu* pMenu = CCMenu::createWithArray(items);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, 10000);
	_anim_index = 0;
	
	_jsons.push_back("SampleAnimations/1/1.json");
	_jsons.push_back("SampleAnimations/2/2.json");
	_jsons.push_back("SampleAnimations/3/3.json");	
	_jsons.push_back("SampleAnimations/4/4.json");
	addObjectsToScene(1);
	black();
	
	setTouchEnabled(true);
	return true;
}

void GafFeatures::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if (!_objects || !_objects->count())
	{
		return;
	}
	CCNode * node = (CCNode*)_objects->objectAtIndex(0);
	CCTouch * pTouch = (CCTouch*) pTouches->anyObject();
	node->setPosition(pTouch->getLocation());
}


void GafFeatures::next_anim()
{
	if (!_jsons.size())
	{
		return;
	}
	cleanup();
	++_anim_index;
	if (_anim_index >= _jsons.size())
	{
		_anim_index = 0;
	}
	addObjectsToScene(1);
		
}

void GafFeatures::prev_anim()
{
	if (!_jsons.size())
	{
		return;
	}
	cleanup();
	--_anim_index;
	if (_anim_index < 0)
	{
		_anim_index = _jsons.size() - 1;
	}
	addObjectsToScene(1);
}

void GafFeatures::restart()
{
	if (!_objects || !_objects->count())
	{
		return;
	}
	GAFAnimatedObject *object = (GAFAnimatedObject *)_objects->objectAtIndex(0);
	object->stop();
	object->start();
}

void GafFeatures::playpause()
{
	if (!_objects || !_objects->count())
	{
		return;
	}
	GAFAnimatedObject *object = (GAFAnimatedObject *)_objects->objectAtIndex(0);
	

	if (object->isAnimationRunning())
	{
		object->pause();
	}
	else
	{
		object->resume();
	}

}

int GafFeatures::maxFrameNumber()
{
	if (!_objects || !_objects->count())
	{
		return -1;
	}
	GAFAnimatedObject *object = (GAFAnimatedObject *)_objects->objectAtIndex(0);
	return object->totalFrameCount();	
}

void GafFeatures::setFrameNumber(int aFrameNumber)
{
	if (!_objects || !_objects->count())
	{
		return;
	}
	GAFAnimatedObject *object = (GAFAnimatedObject *)_objects->objectAtIndex(0);
	object->setFrame(aFrameNumber);	
}

int GafFeatures::frameNumber()
{
	if (!_objects || !_objects->count())
	{
		return -1;
	}
	GAFAnimatedObject *object = (GAFAnimatedObject *)_objects->objectAtIndex(0);
	return object->currentFrameIndex();
}


void GafFeatures::set1()
{
	set(1);
}

void GafFeatures::set5()
{
	set(5);
}

void GafFeatures::set10()
{
	set(10);
}

void GafFeatures::set(int n)
{
	if (!_objects)
	{
		_objects = CCArray::create();
		_objects->retain();
	}
	int c = _objects->count();
	if (c == n)
	{
		return;
	}
	if (n > c)
	{
		addObjectsToScene(n - c);
	}
	else
	{
		removeFromScene(c - n);
	}
}

void GafFeatures::addOne()
{
	addObjectsToScene(1);
}

void GafFeatures::removeOne()
{
	removeFromScene(1);
}

void GafFeatures::cleanup()
{
	CC_SAFE_RELEASE_NULL(_asset);
	if (!_objects)
	{
		return;
	}
	removeFromScene(_objects->count());
	CC_SAFE_RELEASE_NULL(_objects);
}

void GafFeatures::removeFromScene(int aCount)
{
	if (!_objects || !aCount)
	{
		return;
	}
	if (aCount > _objects->count())
	{
		aCount = _objects->count();
	}
	for (int i = 0; i < aCount; ++i)
	{
		GAFAnimatedObject *obj = (GAFAnimatedObject *) _objects->lastObject();
		obj->removeFromParentAndCleanup(true);
		_objects->removeLastObject();
	}
}

void GafFeatures::addObjectsToScene(int aCount)
{
	if (!aCount)
	{
		return;
	}
	if (!_asset)
	{		
		_asset =  GAFAsset::create(_jsons[_anim_index]);
		CC_SAFE_RETAIN(_asset);
	}
	if (!_objects)
	{
		_objects = CCArray::create();
		_objects->retain();
	}
	if (_asset)
	{
		int initialCount = _objects->count();
		for (int i = initialCount; i < initialCount + aCount; ++i)
		{
			GAFAnimatedObject *object = _asset->createObjectAndRun(true);
			object->setZOrder(100 * i);
			addChild(object);
			float s = CCDirector::sharedDirector()->getWinSize().height / 640;
			if (s > 1) s = 1;
			object->setScale(s);			
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			object->setPosition(winSize.width * (0.4 + 0.05 * i), winSize.height * 0.6);
			_objects->addObject(object);
			
			// will work only if animation has sequence
			object->playSequence("walk", true);
		}
	}
}
