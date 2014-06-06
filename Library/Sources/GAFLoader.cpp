#include "GAFPrecompiled.h"
#include "GAFLoader.h"
#include "GAFAsset.h"
#include "DefinitionTagBase.h"
#include "GAFHeader.h"
#include "GAFStream.h"
#include "GAFFile.h"

#include "PrimitiveDeserializer.h"

#include "TagDefineAtlas.h"
#include "TagDefineAnimationMasks.h"
#include "TagDefineAnimationObjects.h"
#include "TagDefineAnimationFrames.h"
#include "TagDefineNamedParts.h"
#include "TagDefineSequences.h"
#include "TagDefineStage.h"
#include "TagDefineAnimationFrames2.h"

void GAFLoader::_readHeaderEnd(GAFHeader& header)
{
    header.framesCount = m_stream->readU16();
    PrimitiveDeserializer::deserialize(m_stream, &header.frameSize);
    PrimitiveDeserializer::deserialize(m_stream, &header.pivot);
}

void GAFLoader::_readHeaderEndV4(GAFHeader& header)
{
    size_t scaleValuesCount = m_stream->readU32();
    while (scaleValuesCount--)
    {
        float val = m_stream->readFloat();
        header.scaleValues.push_back(val);
    }

    size_t csfValuesCount = m_stream->readU32();
    while (csfValuesCount--)
    {
        unsigned short val = m_stream->readU16();
        header.csfValues.push_back(val);
    }
}

void GAFLoader::_registerTagLoadersV3()
{
    m_tagLoaders[Tags::TagDefineAtlas] = new TagDefineAtlas();
    m_tagLoaders[Tags::TagDefineAnimationMasks] = new TagDefineAnimationMasks();
    m_tagLoaders[Tags::TagDefineAnimationObjects] = new TagDefineAnimationObjects();
    m_tagLoaders[Tags::TagDefineAnimationFrames] = new TagDefineAnimationFrames();
}

void GAFLoader::_registerTagLoadersV4()
{
    m_tagLoaders[Tags::TagDefineAnimationFrames2] = new TagDefineAnimationFrames2();
    m_tagLoaders[Tags::TagDefineAnimationObjects2] = new TagDefineAnimationObjects();
    m_tagLoaders[Tags::TagDefineAnimationMasks2] = new TagDefineAnimationMasks();
    m_tagLoaders[Tags::TagDefineAtlas2] = new TagDefineAtlas();
}

void GAFLoader::_registerTagLoadersCommon()
{
    m_tagLoaders[Tags::TagDefineStage] = new TagDefineStage();
    m_tagLoaders[Tags::TagDefineNamedParts] = new TagDefineNamedParts();
    m_tagLoaders[Tags::TagDefineSequences] = new TagDefineSequences();
}

GAFLoader::GAFLoader():
m_stream(NULL)
{
}

GAFLoader::~GAFLoader()
{
    for (TagLoaders_t::iterator i = m_tagLoaders.begin(), e = m_tagLoaders.end(); i != e; ++i)
    {
        delete i->second;
    }
}

bool GAFLoader::loadFile(const std::string& fname, GAFAsset* context)
{
    GAFFile* file = new GAFFile();

    bool retval = false;

    if (file->open(fname, "rb"))
    {
        retval = true;
        m_stream = new GAFStream(file);

        GAFHeader& header = m_stream->getInput()->getHeader();

        if (header.getMajorVersion() == 4)
        {
            _readHeaderEndV4(header);
            _registerTagLoadersV4();
        }
        else
        {
            _readHeaderEnd(header);
            _registerTagLoadersV3();
        }

        _registerTagLoadersCommon();

        context->setHeader(header);

        while (!m_stream->isEndOfStream())
        {
            Tags::Enum tag = m_stream->openTag();

            TagLoaders_t::iterator it = m_tagLoaders.find(tag);

            if (it != m_tagLoaders.end())
            {
                it->second->read(m_stream, context);
            }
            else
            {
                // TODO: show warning
            }

            m_stream->closeTag();
        }

        delete m_stream;
    }

    delete file;

    return retval;
}

bool GAFLoader::isFileLoaded() const
{
    return m_stream && m_stream->isEndOfStream();
}

GAFStream* GAFLoader::getStream() const
{
    return m_stream;
}

const GAFHeader& GAFLoader::getHeader() const
{
    return m_stream->getInput()->getHeader();
}

void GAFLoader::registerTagLoader(unsigned int idx, DefinitionTagBase* tagptr)
{
    m_tagLoaders[static_cast<Tags::Enum>(idx)] = tagptr;
}
