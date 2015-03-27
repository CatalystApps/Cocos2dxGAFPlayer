#include "EventsTest.h"
#include "../testResource.h"
#include "GAFTimelineAction.h"

static std::function<Layer*()> createFunctions[] = {
    CL(CustomEventTest),
};

static int sceneIdx = -1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

DEFAULT_NEXT_ACTION;
DEFAULT_BACK_ACTION;
DEFAULT_RESTART_ACTION;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


bool BaseEventsTest::init()
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

BaseEventsTest::~BaseEventsTest()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

std::string BaseEventsTest::title() const
{
    return "Events Demo";
}

std::string BaseEventsTest::subtitle() const
{
    return "---";
}

TEPLATE_CALLBACK_FUNCTION(BaseEventsTest, back, EventsTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseEventsTest, next, EventsTestScene);
TEPLATE_CALLBACK_FUNCTION(BaseEventsTest, restart, EventsTestScene);

void BaseEventsTest::setup()
{
}

std::string CustomEventTest::subtitle() const
{
    return "Custom event";
}

void CustomEventTest::setup()
{
    _eventDispatcher->addCustomEventListener("ev_name", CC_CALLBACK_1(CustomEventTest::onCustomEvent, this));
}

gaf::GAFObject* CustomEventTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafEvents);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

void CustomEventTest::onCustomEvent(void* params)
{
    auto customEvent = reinterpret_cast<EventCustom*>(params);
    auto gafEvent = reinterpret_cast<gaf::GAFTimelineAction*>(customEvent->getUserData());
    CCLOG(
        "Catched event '%s' with parameter '%s'", 
        gafEvent->getParam(gaf::GAFTimelineAction::PI_EVENT_TYPE).c_str(),
        gafEvent->getParam(gaf::GAFTimelineAction::PI_EVENT_DATA).c_str()
        );
}

/////////////////////////////////////////////////////////////

void EventsTestScene::runThisTest()
{
    auto layer = nextAction();
    addChild(layer);
    Director::getInstance()->replaceScene(this);
}