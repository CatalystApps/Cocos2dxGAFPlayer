#include "AppDelegate.h"

#include "cocos2d.h"
#include "GafFeatures.h"
#include <vector>
#include <string>
#include "GAFShaderManager.h"


static int const kGlobalFPS = 30;
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
    paths.push_back("Shaders");
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

    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    //pEGLView->setDesignResolutionSize(2048, 1536, kResolutionNoBorder);

    CCSize frameSize = pEGLView->getFrameSize();

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
    if (frameSize.height > 768)
    {
        //pDirector->setContentScaleFactor(1536 / 768);
    }
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > 320)
    {
        //pDirector->setContentScaleFactor(768 / 320);
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
    else
    {
        //pDirector->setContentScaleFactor(320 / 1536);
    }

    //pDirector->setContentScaleFactor(2.f);

    // create a scene. it's an autorelease object
    CCScene *pScene = GafFeatures::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    GAFShaderManager::handleEnterBackground();
}

void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
}

#ifdef _WIN32

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
#endif