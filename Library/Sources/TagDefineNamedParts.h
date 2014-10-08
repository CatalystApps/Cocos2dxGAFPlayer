#pragma once

#include "DefinitionTagBase.h"

namespace gaf
{

class TagDefineNamedParts : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);

};

}