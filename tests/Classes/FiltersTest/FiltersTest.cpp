#include "FiltersTest.h"
#include "../testResource.h"

static std::function<Layer*()> createFunctions[] = {
    CL(SampleFiltersTest),
    CL(FiltersWithAlphaTest),
    CL(FiltersWithTintTest),
};

static int sceneIdx = -1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

DEFAULT_NEXT_ACTION;
DEFAULT_BACK_ACTION;
DEFAULT_RESTART_ACTION;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

bool BaseFiltersTest::init()
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

BaseFiltersTest::~BaseFiltersTest()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

std::string BaseFiltersTest::title() const
{
    return "Filters Demo";
}

std::string BaseFiltersTest::subtitle() const
{
    return "---";
}

void BaseFiltersTest::setup()
{
}

TEPLATE_CALLBACK_FUNCTION(BaseFiltersTest, back, FiltersTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseFiltersTest, next, FiltersTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseFiltersTest, restart, FiltersTestScene);

std::string SampleFiltersTest::subtitle() const
{
    return "4 filters on same MovieClip (ring)";
}

gaf::GAFObject* SampleFiltersTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafFiltersSample);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

std::string FiltersWithAlphaTest::subtitle() const
{
    return "Filters + alpha";
}

gaf::GAFObject* FiltersWithAlphaTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafFiltersAlpha);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

std::string FiltersWithTintTest::subtitle() const
{
    return "Filters + tint (yellow)";
}

gaf::GAFObject* FiltersWithTintTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafFiltersTint);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

/////////////////////////////////////////////////////////////

void FiltersTestScene::runThisTest()
{
    auto layer = nextAction();
    addChild(layer);
    Director::getInstance()->replaceScene(this);
}