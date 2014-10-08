#pragma once

#include "DefinitionTagBase.h"

namespace gaf
{

class TagDefineStage : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);

};

}