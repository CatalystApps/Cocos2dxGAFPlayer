#pragma once

#include "DefinitionTagBase.h"

namespace gaf
{

class TagDefineSequences : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);

};

}