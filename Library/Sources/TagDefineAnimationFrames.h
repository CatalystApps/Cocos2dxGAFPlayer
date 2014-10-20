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

    void _translateColor(cocos2d::Color4F& out, const cocos2d::Color4B& in);

public:
    
    ~TagDefineAnimationFrames();
    
    virtual void read(GAFStream*, GAFAsset*);

};

NS_GAF_END
