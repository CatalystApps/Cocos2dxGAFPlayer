#pragma once

class GAFStream;
class GAFAsset;
class GAFTimeline;

class DefinitionTagBase
{
public:
    virtual ~DefinitionTagBase() {};

    virtual void read(GAFStream*, GAFAsset*, GAFTimeline*) = 0;
};