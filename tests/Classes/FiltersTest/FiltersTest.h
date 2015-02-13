#pragma once

#include "../testBasic.h"
#include "../BaseTest.h"

class BaseFiltersTest : public BaseTest
{
public:
    CREATE_FUNC(BaseFiltersTest);

    ~BaseFiltersTest();
    virtual bool init();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup();

    virtual gaf::GAFObject* gaf_object() { return nullptr; }

    DEFAULT_ACTION_CALLBACKS(FiltersTestScene);
};

class SampleFiltersTest : public BaseFiltersTest
{
public:
    CREATE_FUNC(SampleFiltersTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

class FiltersWithAlphaTest : public BaseFiltersTest
{
public:
    CREATE_FUNC(FiltersWithAlphaTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

class FiltersWithTintTest : public BaseFiltersTest
{
public:
    CREATE_FUNC(FiltersWithTintTest);

    virtual std::string subtitle() const override;

    virtual gaf::GAFObject* gaf_object() override;
};

////////////////////////////////////////////
class FiltersTestScene : public TestScene
{
public:
    CREATE_FUNC(FiltersTestScene);

    virtual void runThisTest();
};