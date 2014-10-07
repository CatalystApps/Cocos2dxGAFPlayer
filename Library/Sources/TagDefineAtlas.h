#pragma once

#include "DefinitionTagBase.h"

namespace gaf
{

class TagDefineAtlas : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);
};

}