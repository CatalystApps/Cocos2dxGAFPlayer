#pragma once

#include "DefinitionTagBase.h"

class GAFLoader;

class TagDefineTimeline : public DefinitionTagBase
{
private:
    GAFLoader*  m_loader; // weak
public:

    TagDefineTimeline(GAFLoader*);

    virtual void read(GAFStream*, GAFAsset*);

};