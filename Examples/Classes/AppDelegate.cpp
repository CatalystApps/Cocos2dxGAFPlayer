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