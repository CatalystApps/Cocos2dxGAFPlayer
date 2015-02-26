#include "BundlesTest.h"
#include "../testResource.h"

static std::function<Layer*()> createFunctions[] = {
    CL(ZipBundlesTest),
};

static int sceneIdx = -1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

DEFAULT_NEXT_ACTION;
DEFAULT_BACK_ACTION;
DEFAULT_RESTART_ACTION;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


bool BaseBundlesTest::init()
{
    if (BaseTest::init())
    {
        m_gafObject = gaf_object();
        this->addChild(m_gafObject, -1);

        this->setup();
        return true;
    }
    return false;
}

BaseBundlesTest::~BaseBundlesTest()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

std::string BaseBundlesTest::title() const
{
    return "Bundles Demo";
}

std::string BaseBundlesTest::subtitle() const
{
    return "---";
}

TEPLATE_CALLBACK_FUNCTION(BaseBundlesTest, back, BundlesTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseBundlesTest, next, BundlesTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseBundlesTest, restart, BundlesTestScene);

void BaseBundlesTest::setup()
{
}

std::string ZipBundlesTest::subtitle() const
{
    return "Zip";
}

gaf::GAFObject* ZipBundlesTest::gaf_object()
{
    auto asset = gaf::GAFAsset::createWithBundle(s_gafZipFireman, "fireman/fireman.gaf", nullptr);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

/////////////////////////////////////////////////////////////

void BundlesTestScene::runThisTest()
{
    auto layer = nextAction();
    addChild(layer);
    Director::getInstance()->replaceScene(this);
}