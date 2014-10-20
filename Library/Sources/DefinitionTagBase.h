#pragma once

#include "GAFMacros.h"

NS_GAF_BEGIN

class GAFStream;
class GAFAsset;

class DefinitionTagBase
{
public:
    virtual ~DefinitionTagBase() {};

    virtual void read(GAFStream*, GAFAsset*) = 0;
};

NS_GAF_END