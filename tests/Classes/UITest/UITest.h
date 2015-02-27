#pragma once

#include "../testBasic.h"
#include "../BaseTest.h"

class BaseUITest : public BaseTest
{
public:
    CREATE_FUNC(BaseUITest);

    ~BaseUITest();
    virtual bool init() override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup();

    virtual gaf::GAFObject* gaf_object() { return nullptr; }

    DEFAULT_ACTION_CALLBACKS(UITestScene);

private:
    gaf::GAFObject* m_gafObject;
};

class TextFieldTest : public BaseUITest
{
public:
    CREATE_FUNC(TextFieldTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

////////////////////////////////////////////
class UITestScene : public TestScene
{
public:
    CREATE_FUNC(UITestScene);

    virtual void runThisTest();
};