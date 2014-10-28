#pragma once

#include "DefinitionTagBase.h"

NS_GAF_BEGIN

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

NS_GAF_END