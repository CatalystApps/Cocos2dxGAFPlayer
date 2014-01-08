//
//  com_dracollc_sphereAppDelegate.cpp
//  com.dracollc.sphere
//
//  Created by Alexander Kalinin on 2/2/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "GafFeatures.h"
#include <vector>
#include <string>

static int const kGlobalFPS = 30.0;
USING_NS_CC;

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
	//paths.push_back("Shaders");
    paths.push_back("Resources");
	
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
}

void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
}

int WINAPI WinMain(__in HINSTANCE hInstance,
    __in_opt HINSTANCE hPrevInstance,
    __in LPSTR lpCmdLine,
    __in int nShowCmd
    )
{
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("GAF Animation sample");
    eglView->setFrameSize(1024, 768);

    return CCApplication::sharedApplication()->run();
}