//
//  com_dracollc_sphereAppDelegate.cpp
//  com.dracollc.sphere
//
//  Created by Alexander Kalinin on 2/2/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GafFeatures.h"
#include <vector>
#include <string>

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
    // initialize director
	std::vector<std::string> paths;
	paths.push_back("Shaders");
	
	CCFileUtils::sharedFileUtils()->setSearchPaths(paths);
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);
	pDirector->setProjection(kCCDirectorProjection2D);
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);

    pDirector->setAnimationInterval(1.0 / kGlobalFPS);

    // create a scene. it's an autorelease object
    CCScene *pScene = GafFeatures::scene();

    // run
    pDirector->runWithScene(pScene);


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}


void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
