#pragma once

#include "../testBasic.h"
#include "../BaseTest.h"

class BaseMaskTimelineTest : public BaseTest
{
public:
    CREATE_FUNC(BaseMaskTimelineTest);

    ~BaseMaskTimelineTest();
    virtual bool init() override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup();

    virtual gaf::GAFObject* gaf_object() { return nullptr; }

    DEFAULT_ACTION_CALLBACKS(MaskTimelineTestScene);
};

class SampleMaskTimelineTest : public BaseMaskTimelineTest
{
public:
    CREATE_FUNC(SampleMaskTimelineTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

class TimelineOverMaskTest : public BaseMaskTimelineTest
{
public:
    CREATE_FUNC(TimelineOverMaskTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

class TimelineInMaskTest : public BaseMaskTimelineTest
{
public:
    CREATE_FUNC(TimelineInMaskTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

////////////////////////////////////////////
class MaskTimelineTestScene : public TestScene
{
public:
    CREATE_FUNC(MaskTimelineTestScene);

    virtual void runThisTest() override;
};