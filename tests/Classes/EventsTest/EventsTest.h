#pragma once

#include "../testBasic.h"
#include "../BaseTest.h"

class BaseEventsTest : public BaseTest
{
public:
    CREATE_FUNC(BaseEventsTest);

    ~BaseEventsTest();
    virtual bool init() override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup();

    virtual gaf::GAFObject* gaf_object() { return nullptr; }

    DEFAULT_ACTION_CALLBACKS(BundlesTestScene);

private:
    gaf::GAFObject* m_gafObject;
};

class CustomEventTest : public BaseEventsTest
{
public:
    CREATE_FUNC(CustomEventTest);

    virtual std::string subtitle() const override;

    virtual void setup() override;

    virtual gaf::GAFObject* gaf_object() override;

    void onCustomEvent(void* params);
};

////////////////////////////////////////////
class EventsTestScene : public TestScene
{
public:
    CREATE_FUNC(EventsTestScene);

    virtual void runThisTest() override;
};