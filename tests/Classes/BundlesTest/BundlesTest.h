#pragma once

#include "../testBasic.h"
#include "../BaseTest.h"

class BaseBundlesTest : public BaseTest
{
public:
    CREATE_FUNC(BaseBundlesTest);

    ~BaseBundlesTest();
    virtual bool init() override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup();

    virtual gaf::GAFObject* gaf_object() { return nullptr; }

    DEFAULT_ACTION_CALLBACKS(BundlesTestScene);

private:
    gaf::GAFObject* m_gafObject;
};

class ZipBundlesTest : public BaseBundlesTest
{
public:
    CREATE_FUNC(ZipBundlesTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

////////////////////////////////////////////
class BundlesTestScene : public TestScene
{
public:
    CREATE_FUNC(BundlesTestScene);

    virtual void runThisTest();
};