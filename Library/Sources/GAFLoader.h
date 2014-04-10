#pragma once

#include "TagDefines.h"

class GAFAsset;

class GAFStream;
class DefinitionTagBase;
class GAFHeader;

class GAFLoader
{
private:
    GAFStream*           m_stream;

    void                 _readHeaderEnd(GAFHeader&);
    void                 _registerTagLoaders();

    typedef std::map<Tags::Enum, DefinitionTagBase*> TagLoaders_t;

    TagLoaders_t         m_tagLoaders;

public:
    GAFLoader();
    ~GAFLoader();

    bool                 loadFile(const std::string& fname, GAFAsset* context);
    bool                 isFileLoaded() const;

    GAFStream*           getStream() const;

    const GAFHeader&     getHeader() const;

};