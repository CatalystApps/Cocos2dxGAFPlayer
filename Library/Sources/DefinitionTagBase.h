#pragma once

NS_GAF_BEGIN

class GAFStream;
class GAFAsset;
class GAFTimeline;

class DefinitionTagBase
{
public:
    virtual ~DefinitionTagBase() {};

    virtual void read(GAFStream*, GAFAsset*, GAFTimeline*) = 0;
};

NS_GAF_END