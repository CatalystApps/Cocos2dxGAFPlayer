#pragma once

#include "../testBasic.h"
#include "../BaseTest.h"

class BaseFramePlaybackTest : public BaseTest
{
public:
    CREATE_FUNC(BaseFramePlaybackTest);

    ~BaseFramePlaybackTest();
    virtual bool init();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup();

    virtual gaf::GAFObject* gaf_object() { return nullptr; }

    DEFAULT_ACTION_CALLBACKS(FramePlaybackTestScene);
};

class DirectFramePlaybackTest : public BaseFramePlaybackTest
{
public:
    CREATE_FUNC(DirectFramePlaybackTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

class ReverseFramePlaybackTest : public BaseFramePlaybackTest
{
public:
    CREATE_FUNC(ReverseFramePlaybackTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

////////////////////////////////////////////
class FramePlaybackTestScene : public TestScene
{
public:
    CREATE_FUNC(FramePlaybackTestScene);

    virtual void runThisTest();
};