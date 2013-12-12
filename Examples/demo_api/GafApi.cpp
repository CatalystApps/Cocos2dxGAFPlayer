#include "GafApi.h"
#include "SimpleAudioEngine.h"
#include "GAFAnimatedObject.h"
#include "GAFAsset.h"
#include "GAFSprite.h"

using namespace cocos2d;
using namespace CocosDenshion;

static const char * kAnimationPath          = "SampleAnimations/4/4.json";
static const char * kNamedAnimationPath     = "SampleAnimations/4/5.json";

GafApi::GafApi()
:
_asset(NULL)
{
}
GafApi::~GafApi()
{
	CC_SAFE_RELEASE(_asset);
}

CCScene* GafApi::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GafApi *layer = GafApi::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

CCMenuItemImage * GafApi::addBtn(const char * text, float px, float py, SEL_MenuHandler handler, float k)
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
	
	// position the label on the center of the screen
	pLabel->setAnchorPoint(CCPoint(1, 0.5));
	pLabel->setScale(k);
    pLabel->setPosition( ccp(res->getPositionX() - res->getContentSize().width * k * 0.5, res->getPositionY()) );
	
    // add the label as a child to this layer
    addChild(pLabel, 100000);
	return res;
}

void GafApi::black()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GafApi::white()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GafApi::gray()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}


bool GafApi::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	CCSize size = CCDirector::sharedDirector()->getWinSizeInPixels();
	
	float s = size.height / 640;
	if (s > 1) s = 1;
//	float dp = 0.13;
	
	CCArray * items = CCArray::create();

	items->addObject(addBtn("B", 0.75, 0.95, menu_selector(GafApi::black), s));
	items->addObject(addBtn("W", 0.85, 0.95, menu_selector(GafApi::white), s));
	items->addObject(addBtn("G", 0.95, 0.95, menu_selector(GafApi::gray), s));

    CCMenu* pMenu = CCMenu::createWithArray(items);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, 10000);

	demo();
	white();
	setTouchEnabled(true);
	

	return true;
}

void GafApi::demo()
{
	// one-liner to create animation and run it
	GAFAnimatedObject * object = GAFAnimatedObject::createAndRun(kAnimationPath, true);
	addToScene(object, 0.1, 0.6);

	object->setTag(10);
	object->setSequenceDelegate(this);

	// using GAFAsset to spawn multiple instances of GAFAnimatedObject
	_asset = GAFAsset::create(kAnimationPath);
	
	object = _asset->createObjectAndRun();
	addToScene(object, 0.4, 0.2, "walk");

	
	addToScene(_asset->createObjectAndRun(), 0.4, 0.5, "attack");
	addToScene(_asset->createObjectAndRun(), 0.4, 0.8, "death");
	
	
	object = GAFAnimatedObject::createAndRun(kNamedAnimationPath, true);
	// you can set name of any movieclip in flash and, sprite in code, and use in any way
	GAFSprite * blue = object->subobjectByName("Blue");
	addToScene(object, 0.5, 0.5);
	if (blue)
	{
		GAFAnimatedObject * pathObject = _asset->createObjectAndRun(true);
		// common pattern to attach nodes to your animation is making attachment child of
		// named GAFSprite and settting that sprite as locator
		blue->addChild(pathObject);
		pathObject->setPosition(CCPointZero);
		blue->setLocator(true);
	}
}

void GafApi::addToScene(GAFAnimatedObject * object, float px, float py, const char * sequenceName)
{
	addChild(object);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	object->setPosition(ccp(size.width * px, size.height * py));	
	if (sequenceName)
	{
		object->playSequence(sequenceName, true);
	}
}

void GafApi::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}

void GafApi::onFinishSequence(GAFAnimatedObject * object, const std::string& sequenceName)
{
	printf("onFinishSequence TAG:%d NAME:%s\n", object->getTag(), sequenceName.c_str());
}

