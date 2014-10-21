#pragma once

#include "DefinitionTagBase.h"

NS_GAF_BEGIN

class GAFSubobjectState;

class TagDefineAnimationFrames2 : public DefinitionTagBase
{
private:
    GAFSubobjectState* extractState(GAFStream* in);
public:

    virtual void read(GAFStream*, GAFAsset*);

};

NS_GAF_END