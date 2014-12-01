#pragma once

#include "DefinitionTagBase.h"

NS_GAF_BEGIN

class GAFSubobjectState;

class TagDefineAnimationFrames : public DefinitionTagBase
{
private:
    typedef std::map<unsigned int, GAFSubobjectState*> States_t;
    States_t m_currentStates;
    
    GAFSubobjectState* extractState(GAFStream* in);

public:
    
    ~TagDefineAnimationFrames();
    
    virtual void read(GAFStream*, GAFAsset*, GAFTimeline*);

};

NS_GAF_END
