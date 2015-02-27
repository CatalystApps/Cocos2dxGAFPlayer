#include "UITest.h"
#include "../testResource.h"

static std::function<Layer*()> createFunctions[] = {
    CL(TextFieldTest),
};

static int sceneIdx = -1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

DEFAULT_NEXT_ACTION;
DEFAULT_BACK_ACTION;
DEFAULT_RESTART_ACTION;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


bool BaseUITest::init()
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

BaseUITest::~BaseUITest()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

std::string BaseUITest::title() const
{
    return "UI Demo";
}

std::string BaseUITest::subtitle() const
{
    return "---";
}

TEPLATE_CALLBACK_FUNCTION(BaseUITest, back, UITestScene);
TEPLATE_CALLBACK_FUNCTION(BaseUITest, next, UITestScene);
TEPLATE_CALLBACK_FUNCTION(BaseUITest, restart, UITestScene);

void BaseUITest::setup()
{
}

std::string TextFieldTest::subtitle() const
{
    return "TextFields: STATIC - red, DYNAMIC - blue";
}

gaf::GAFObject* TextFieldTest::gaf_object()
{
    auto asset = gaf::GAFAsset::create(s_gafUITextField);
    auto object = asset->createObjectAndRun(true);
    object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    object->setPosition(Director::getInstance()->getVisibleSize() / 2);

    return object;
}

/////////////////////////////////////////////////////////////

void UITestScene::runThisTest()
{
    auto layer = nextAction();
    addChild(layer);
    Director::getInstance()->replaceScene(this);
}