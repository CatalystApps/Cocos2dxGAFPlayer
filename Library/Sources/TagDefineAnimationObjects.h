#pragma once

#include "DefinitionTagBase.h"

namespace gaf
{

class TagDefineAnimationObjects : public DefinitionTagBase
{
public:
    virtual void read(GAFStream*, GAFAsset*);
};

}