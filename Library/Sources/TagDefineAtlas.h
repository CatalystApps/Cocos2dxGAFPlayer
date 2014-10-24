#pragma once

#include "DefinitionTagBase.h"

NS_GAF_BEGIN

class TagDefineAtlas : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);
};

NS_GAF_END
