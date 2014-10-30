#include "GAFPrecompiled.h"
#include "GAFLoader.h"
#include "GAFAsset.h"
#include "DefinitionTagBase.h"
#include "GAFHeader.h"
#include "GAFStream.h"
#include "GAFFile.h"
#include "GAFTimeline.h"

#include "PrimitiveDeserializer.h"

#include "TagDefineAtlas.h"
#include "TagDefineAnimationMasks.h"
#include "TagDefineAnimationObjects.h"
#include "TagDefineAnimationFrames.h"
#include "TagDefineNamedParts.h"
#include "TagDefineSequences.h"
#include "TagDefineStage.h"
#include "TagDefineAnimationFrames2.h"
#include "TagDefineTimeline.h"
#include "TagDefineTextField.h"

NS_GAF_BEGIN

void GAFLoader::_readHeaderEnd(GAFHeader& header)
{
    header.framesCount = m_stream->readU16();
    PrimitiveDeserializer::deserialize(m_stream, &header.frameSize);
    PrimitiveDeserializer::deserialize(m_stream, &header.pivot);
}

void GAFLoader::_readHeaderEndV4(GAFHeader& header)
{
    size_t scaleValuesCount = m_stream->readU32();
    while (scaleValuesCount)
    {
        float val = m_stream->readFloat();
        header.scaleValues.push_back(val);

        scaleValuesCount--;
    }

    size_t csfValuesCount = m_stream->readU32();
    while (csfValuesCount)
    {
        float val = m_stream->readFloat();
        header.csfValues.push_back(val);

        csfValuesCount--;
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
    m_tagLoaders[Tags::TagDefineTextFields] = new TagDefineTextField();
    m_tagLoaders[Tags::TagDefineTimeline] = new TagDefineTimeline(this);
}

void GAFLoader::_registerTagLoadersCommon()
{
    m_tagLoaders[Tags::TagDefineStage] = new TagDefineStage();
    m_tagLoaders[Tags::TagDefineNamedParts] = new TagDefineNamedParts();
    m_tagLoaders[Tags::TagDefineSequences] = new TagDefineSequences();
}

GAFLoader::GAFLoader():
m_stream(nullptr)
{
}

GAFLoader::~GAFLoader()
{
    for (TagLoaders_t::iterator i = m_tagLoaders.begin(), e = m_tagLoaders.end(); i != e; ++i)
    {
        delete i->second;
    }
}

void GAFLoader::loadTags(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    bool tagEndRead = false;

    while (!in->isEndOfStream())
    {
        Tags::Enum tag = in->openTag();

        TagLoaders_t::iterator it = m_tagLoaders.find(tag);

        if (it != m_tagLoaders.end())
        {
            it->second->read(in, asset, timeline);
        }
        else
        {
            // TODO: show warning
        }

        in->closeTag();

        if (tag == Tags::TagEnd)
        {
            tagEndRead = true;
            break;
        }
    }

    if (!tagEndRead)
    {
        //TODO: warning or error here
    }
}

bool GAFLoader::loadData(const unsigned char* data, size_t len, GAFAsset* context)
{
    GAFFile* file = new GAFFile();

    bool retval = false;

    if (file->open(data, len))
    {
        retval = true;

        _processLoad(file, context);
    }

    delete file;

    return retval;
}

void GAFLoader::_processLoad(GAFFile* file, GAFAsset* context)
{
    m_stream = new GAFStream(file);

    GAFHeader& header = m_stream->getInput()->getHeader();

	GAFTimeline *timeline = nullptr;
    if (header.getMajorVersion() == 4)
    {
        _readHeaderEndV4(header);
        _registerTagLoadersV4();
    }
    else
    {
        _readHeaderEnd(header);
        _registerTagLoadersV3();

		timeline = new GAFTimeline(nullptr, 0, header.frameSize, header.pivot, header.framesCount);
		context->setRootTimeline(timeline);
		context->pushTimeline(0, timeline);
    }

    _registerTagLoadersCommon();

    context->setHeader(header);

    loadTags(m_stream, context, timeline);

    delete m_stream;
}

bool GAFLoader::loadFile(const std::string& fname, GAFAsset* context)
{
    GAFFile* file = new GAFFile();

    bool retval = false;

    if (file->open(fname, "rb"))
    {
        retval = true;

        _processLoad(file, context);
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

NS_GAF_END