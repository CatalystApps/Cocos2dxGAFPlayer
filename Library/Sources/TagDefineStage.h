#pragma once

#include "DefinitionTagBase.h"

class TagDefineStage : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);

};