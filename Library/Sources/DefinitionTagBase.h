#pragma once

class GAFStream;
class GAFAsset;

class DefinitionTagBase
{
public:
    virtual ~DefinitionTagBase() {};

    virtual void read(GAFStream*, GAFAsset*) = 0;
};