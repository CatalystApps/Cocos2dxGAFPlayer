#pragma once

#include "DefinitionTagBase.h"

NS_GAF_BEGIN

class TagDefineStage : public DefinitionTagBase
{
public:

    virtual void read(GAFStream*, GAFAsset*, GAFTimeline*);

};

NS_GAF_END