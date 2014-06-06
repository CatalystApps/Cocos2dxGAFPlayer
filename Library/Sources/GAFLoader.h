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
    void                 _readHeaderEndV4(GAFHeader&);

    void                 _registerTagLoadersV3();
    void                 _registerTagLoadersCommon();
    void                 _registerTagLoadersV4();

    typedef std::map<Tags::Enum, DefinitionTagBase*> TagLoaders_t;

    TagLoaders_t         m_tagLoaders;

public:
    GAFLoader();
    ~GAFLoader();

    bool                 loadFile(const std::string& fname, GAFAsset* context);
    bool                 isFileLoaded() const;

    GAFStream*           getStream() const;

    const GAFHeader&     getHeader() const;

    int                  getVersionMinor() const;
    int                  getVersionMajor() const;

    void                 registerTagLoader(unsigned int idx, DefinitionTagBase*);

    void                 loadTags(GAFStream* in, GAFAsset* context);
};