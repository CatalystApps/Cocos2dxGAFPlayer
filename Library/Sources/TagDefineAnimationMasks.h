#pragma once

#include "DefinitionTagBase.h"

namespace gaf
{

class TagDefineAnimationMasks : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);

};

}