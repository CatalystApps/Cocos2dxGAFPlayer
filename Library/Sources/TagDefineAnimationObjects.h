#pragma once

#include "DefinitionTagBase.h"

class TagDefineAnimationObjects : public DefinitionTagBase
{
public:
    virtual void read(GAFStream*, GAFAsset*);
};