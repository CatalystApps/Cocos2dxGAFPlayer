#pragma once

#include "DefinitionTagBase.h"

class TagDefineNamedParts : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);

};