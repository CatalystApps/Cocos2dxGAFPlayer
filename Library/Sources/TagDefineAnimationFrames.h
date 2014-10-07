#pragma once

#include "DefinitionTagBase.h"

namespace gaf
{

class GAFSubobjectState;

class TagDefineAnimationFrames : public DefinitionTagBase
{
private:
    GAFSubobjectState* extractState(GAFStream* in);

    void _translateColor(cocos2d::Color4F& out, const cocos2d::Color4B& in);

public:

    virtual void read(GAFStream*, GAFAsset*);

};

}