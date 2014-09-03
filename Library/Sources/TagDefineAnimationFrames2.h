#pragma once

#include "DefinitionTagBase.h"

class GAFSubobjectState;

class TagDefineAnimationFrames2 : public DefinitionTagBase
{
private:
    GAFSubobjectState* extractState(GAFStream* in);
    
    typedef std::map<unsigned int, GAFSubobjectState*> States_t;
    States_t m_currentStates;
public:
	
    ~TagDefineAnimationFrames2();
    
    virtual void read(GAFStream*, GAFAsset*, GAFTimeline*);

};