#pragma once

#include "DefinitionTagBase.h"

NS_GAF_BEGIN

class TagDefineNamedParts : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*);

};

NS_GAF_END