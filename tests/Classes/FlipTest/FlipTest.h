#pragma once

#include "../testBasic.h"
#include "../BaseTest.h"

class BaseFlipTest : public BaseTest
{
public:
    CREATE_FUNC(BaseFlipTest);

    ~BaseFlipTest();
    virtual bool init() override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup();

    virtual gaf::GAFObject* gaf_object() { return nullptr; }

    DEFAULT_ACTION_CALLBACKS(FramePlaybackTestScene);

    void onTouchesBegan(const std::vector<Touch*>& touches, Event *event) override;

private:
    gaf::GAFObject* m_gafObject;
};

class DefaultCsfFlipTest : public BaseFlipTest
{
public:
    CREATE_FUNC(DefaultCsfFlipTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

class Csf2FlipTest : public BaseFlipTest
{
public:
    CREATE_FUNC(Csf2FlipTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

////////////////////////////////////////////
class FlipTestScene : public TestScene
{
public:
    CREATE_FUNC(FlipTestScene);

    virtual void runThisTest();
};