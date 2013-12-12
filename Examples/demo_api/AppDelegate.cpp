#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GafApi.h"
#include <vector>
#include <string>
#include "GAFShaderManager.h"

static int const kGlobalFPS = 30.0;
USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	std::vector<std::string> paths;
	paths.push_back("Shaders");
	CCFileUtils::sharedFileUtils()->setSearchPaths(paths);
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    pDirector->setDisplayStats(true);
	pDirector->setProjection(kCCDirectorProjection2D);
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    pDirector->setAnimationInterval(1.0 / kGlobalFPS);
    CCScene *pScene = GafApi::scene();
    pDirector->runWithScene(pScene);
    return true;
}
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	GAFShaderManager::handleEnterBackground();
}


void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
