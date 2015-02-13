#pragma once

#include "cocos2d.h"
#include "GAF.h"

class BaseTest : public cocos2d::Layer
{
public:
    virtual std::string title() const;
    virtual std::string subtitle() const;

	virtual void restartCallback(Ref* sender);
	virtual void nextCallback(Ref* sender);
	virtual void backCallback(Ref* sender);

    virtual void onEnter() override;
    virtual void onExit() override;
};