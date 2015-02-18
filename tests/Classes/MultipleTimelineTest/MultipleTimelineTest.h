#pragma once

#include "../testBasic.h"
#include "../BaseTest.h"

class MultipleTimelineTest : public BaseTest
{
public:
    CREATE_FUNC(MultipleTimelineTest);

    ~MultipleTimelineTest();
    virtual bool init();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup();

    virtual gaf::GAFObject* gaf_object();

    DEFAULT_ACTION_CALLBACKS(MaskTimelineTestScene);

    void onTouchesBegan(const std::vector<Touch*>& touches, Event *event) override;

private:
    enum eScene
    {
        SCENE_DEFAULT = 0,
        SCENE_TIMELINE_1,
        SCENE_TIMELINE_2,
        ///
        SCENE_COUNT
    };

    eScene m_scene;
    gaf::GAFObject* m_gafObject;
};

////////////////////////////////////////////
class MultipleTimelineTestScene : public TestScene
{
public:
    CREATE_FUNC(MultipleTimelineTestScene);

    virtual void runThisTest();
};