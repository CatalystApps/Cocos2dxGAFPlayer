#pragma once

#include "DefinitionTagBase.h"

NS_GAF_BEGIN

class GAFLoader;

class TagDefineTimeline : public DefinitionTagBase
{
private:
    GAFLoader*  m_loader; // weak
public:

    TagDefineTimeline(GAFLoader*);

    virtual void read(GAFStream*, GAFAsset*, GAFTimeline*);

};

NS_GAF_END