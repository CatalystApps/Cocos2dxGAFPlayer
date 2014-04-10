#pragma once

#include "DefinitionTagBase.h"

class GAFSubobjectState;

class TagDefineAnimationFrames : public DefinitionTagBase
{
private:
    GAFSubobjectState* extractState(GAFStream* in);

    void _translateColor(ccColor4F& out, const ccColor4B& in);

public:

    virtual void read(GAFStream*, GAFAsset*);

};