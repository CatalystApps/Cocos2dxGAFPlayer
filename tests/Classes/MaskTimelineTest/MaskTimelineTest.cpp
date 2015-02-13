#include "MaskTimelineTest.h"
#include "../testResource.h"

static std::function<Layer*()> createFunctions[] = {
    CL(SampleMaskTimelineTest),
    CL(TimelineOverMaskTest),
    CL(TimelineInMaskTest),
};

static int sceneIdx = -1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

DEFAULT_NEXT_ACTION;
DEFAULT_BACK_ACTION;
DEFAULT_RESTART_ACTION;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

bool BaseMaskTimelineTest::init()
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

BaseMaskTimelineTest::~BaseMaskTimelineTest()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

std::string BaseMaskTimelineTest::title() const
{
    return "Masks Demo";
}

std::string BaseMaskTimelineTest::subtitle() const
{
    return "---";
}

void BaseMaskTimelineTest::setup()
{
}

TEPLATE_CALLBACK_FUNCTION(BaseMaskTimelineTest, back, MaskTimelineTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseMaskTimelineTest, next, MaskTimelineTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseMaskTimelineTest, restart, MaskTimelineTestScene);

std::string SampleMaskTimelineTest::subtitle() const
{
    return "Static sample displaing mask form. Next two tests should be similar";
}

gaf::GAFObject* SampleMaskTimelineTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafMaskSample);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

std::string TimelineOverMaskTest::subtitle() const
{
    return "Timeline over mask";
}

gaf::GAFObject* TimelineOverMaskTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafMasked1);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

std::string TimelineInMaskTest::subtitle() const
{
    return "Timeline in mask";
}

gaf::GAFObject* TimelineInMaskTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafMasked2);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

/////////////////////////////////////////////////////////////

void MaskTimelineTestScene::runThisTest()
{
    auto layer = nextAction();
    addChild(layer);
    Director::getInstance()->replaceScene(this);
}