#include "FramePlaybackTest.h"
#include "../testResource.h"

static std::function<Layer*()> createFunctions[] = {
    CL(DirectFramePlaybackTest),
    CL(ReverseFramePlaybackTest),
};

static int sceneIdx = -1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

DEFAULT_NEXT_ACTION;
DEFAULT_BACK_ACTION;
DEFAULT_RESTART_ACTION;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

bool BaseFramePlaybackTest::init()
{
    if (BaseTest::init())
    {
        auto object = gaf_object();
        this->addChild(object, -1);

        this->setup();
        return true;
    }
    return false;
}

BaseFramePlaybackTest::~BaseFramePlaybackTest()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

std::string BaseFramePlaybackTest::title() const
{
    return "Playback Demo";
}

std::string BaseFramePlaybackTest::subtitle() const
{
    return "---";
}

void BaseFramePlaybackTest::setup()
{
}

TEPLATE_CALLBACK_FUNCTION(BaseFramePlaybackTest, back, FramePlaybackTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseFramePlaybackTest, next, FramePlaybackTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseFramePlaybackTest, restart, FramePlaybackTestScene);

std::string DirectFramePlaybackTest::subtitle() const
{
    return "Direct Playback";
}

gaf::GAFObject* DirectFramePlaybackTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafPlayback1);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

std::string ReverseFramePlaybackTest::subtitle() const
{
    return "Reverse playback";
}

gaf::GAFObject* ReverseFramePlaybackTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafPlayback1);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);
    object->setReversed(true);

    return object;
}

/////////////////////////////////////////////////////////////

void FramePlaybackTestScene::runThisTest()
{
    auto layer = nextAction();
    addChild(layer);
    Director::getInstance()->replaceScene(this);
}