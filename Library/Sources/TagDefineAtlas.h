#pragma once

#include "DefinitionTagBase.h"

class TagDefineAtlas : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);
};