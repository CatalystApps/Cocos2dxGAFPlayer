#pragma once

#include "cocos2d.h"
#include "audio/include/SimpleAudioEngine.h"
#include "VisibleRect.h"

USING_NS_CC;

class TestScene : public Scene
{
public: 
    TestScene(bool bPortrait = false, bool physics = false);
    virtual void onEnter() override;

    virtual void runThisTest() = 0;
};

// C++ 11

#define CL(__className__) [](){ return __className__::create();}
#define CLN(__className__) [](){ auto obj = new __className__(); obj->autorelease(); return obj; }

#define DEFAULT_NEXT_ACTION static Layer* nextAction() { \
    sceneIdx++;                                          \
    sceneIdx = sceneIdx % MAX_LAYER;                     \
    auto layer = (createFunctions[sceneIdx])();          \
    return layer; }

#define DEFAULT_BACK_ACTION static Layer* backAction() { \
    sceneIdx--;                                          \
    if (sceneIdx < 0) sceneIdx += MAX_LAYER;             \
    auto layer = (createFunctions[sceneIdx])();          \
    return layer; }

#define DEFAULT_RESTART_ACTION static Layer* restartAction() { auto layer = (createFunctions[sceneIdx])(); return layer; }

#define TEPLATE_CALLBACK_FUNCTION(__class__, __action__, __scene__) void __class__::__action__##Callback(Ref* sender) { \
    Scene *s = new (std::nothrow) __scene__(); \
    s->addChild(__action__##Action()); Director::getInstance()->replaceScene(s); s->release(); }

#define DEFAULT_ACTION_CALLBACKS(__scene__) void backCallback(Ref* sender); void nextCallback(Ref* sender); void restartCallback(Ref* sender);
