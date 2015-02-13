#include "FlipTest.h"
#include "../testResource.h"

static std::function<Layer*()> createFunctions[] = {
    CL(DefaultCsfFlipTest),
    CL(Csf2FlipTest),
};

static int sceneIdx = -1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

DEFAULT_NEXT_ACTION;
DEFAULT_BACK_ACTION;
DEFAULT_RESTART_ACTION;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


bool BaseFlipTest::init()
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

BaseFlipTest::~BaseFlipTest()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

std::string BaseFlipTest::title() const
{
    return "Flip Demo";
}

std::string BaseFlipTest::subtitle() const
{
    return "---";
}

TEPLATE_CALLBACK_FUNCTION(BaseFlipTest, back, FlipTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseFlipTest, next, FlipTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseFlipTest, restart, FlipTestScene);

void BaseFlipTest::setup()
{
    // show gaf anchor
    auto shape = DrawNode::create();
    Color4F red(1, 0, 0, 1);
    shape->drawDot(Vec2::ZERO, 5, red);

    auto pos = m_gafObject->getAnchorPointInPoints();
    pos = m_gafObject->convertToWorldSpace(pos);
    shape->setPosition(pos);

    this->addChild(shape, 1);

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(BaseFlipTest::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void BaseFlipTest::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    static int flipMask = 0; // 00 01 10 11
    flipMask++;
    flipMask %= 4;

    m_gafObject->setFlippedX(flipMask & (1 << 0));
    m_gafObject->setFlippedY(flipMask & (1 << 1));
}

std::string DefaultCsfFlipTest::subtitle() const
{
    return "Csf - default";
}

gaf::GAFObject* DefaultCsfFlipTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafStandart1);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

std::string Csf2FlipTest::subtitle() const
{
    return "Csf = 2";
}

gaf::GAFObject* Csf2FlipTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafStandart1);
    asset->setDesiredAtlasScale(2);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

/////////////////////////////////////////////////////////////

void FlipTestScene::runThisTest()
{
    auto layer = nextAction();
    addChild(layer);
    Director::getInstance()->replaceScene(this);
}