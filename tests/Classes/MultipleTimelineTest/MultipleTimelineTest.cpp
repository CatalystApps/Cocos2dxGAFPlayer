#include "MultipleTimelineTest.h"
#include "../testResource.h"

static std::function<Layer*()> createFunctions[] = {
    CL(MultipleTimelineTest),
};

static int sceneIdx = -1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

DEFAULT_NEXT_ACTION;
DEFAULT_BACK_ACTION;
DEFAULT_RESTART_ACTION;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

bool MultipleTimelineTest::init()
{
    if (BaseTest::init())
    {
        m_scene = SCENE_DEFAULT;

        m_gafObject = gaf_object();
        this->addChild(m_gafObject, -1);

        this->setup();
        return true;
    }
    return false;
}

MultipleTimelineTest::~MultipleTimelineTest()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

std::string MultipleTimelineTest::title() const
{
    return "Multiple Timelines Demo";
}

std::string MultipleTimelineTest::subtitle() const
{
    return "Tap to switch MainScene->Timeline1->Timeline2";
}

void MultipleTimelineTest::setup()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(MultipleTimelineTest::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

gaf::GAFObject* MultipleTimelineTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafMultipleTimelines);

    switch (m_scene)
    {
    case SCENE_TIMELINE_1:
        asset->setRootTimeline("timeline1");
        break;
    case SCENE_TIMELINE_2: 
        asset->setRootTimeline("timeline2");
        break;
    case SCENE_COUNT:
        CCASSERT(false, "Wrong scene switch logic");
        break;
    case SCENE_DEFAULT:
    default:
        break;
    }

    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

void MultipleTimelineTest::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    m_scene = eScene(m_scene + 1);
    if (m_scene == SCENE_COUNT)
    {
        m_scene = SCENE_DEFAULT;
    }

    this->removeChild(m_gafObject);
    m_gafObject = gaf_object();
    this->addChild(m_gafObject, -1);
}

TEPLATE_CALLBACK_FUNCTION(MultipleTimelineTest, back, MultipleTimelineTestScene);
TEPLATE_CALLBACK_FUNCTION(MultipleTimelineTest, next, MultipleTimelineTestScene);
TEPLATE_CALLBACK_FUNCTION(MultipleTimelineTest, restart, MultipleTimelineTestScene);

/////////////////////////////////////////////////////////////

void MultipleTimelineTestScene::runThisTest()
{
    auto layer = nextAction();
    addChild(layer);
    Director::getInstance()->replaceScene(this);
}