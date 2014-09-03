#pragma once

#include "DefinitionTagBase.h"

class GAFSubobjectState;

class TagDefineAnimationFrames2 : public DefinitionTagBase
{
private:
    GAFSubobjectState* extractState(GAFStream* in);
public:
	// TODO: destructor
    virtual void read(GAFStream*, GAFAsset*, GAFTimeline*);

};