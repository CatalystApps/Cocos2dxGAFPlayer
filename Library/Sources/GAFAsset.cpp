#include "GAFPrecompiled.h"
#include "GAFAsset.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFAnimationFrame.h"
#include "GAFSubobjectState.h"
#include "GAFAnimationSequence.h"
#include "GAFAnimatedObject.h"
#include "GAFShaderManager.h"
#include "GAFLoader.h"

NS_GAF_BEGIN

static float _currentDeviceScale = 1.0f;
static float  _desiredCsf = 1.f;

float GAFAsset::desiredCsf()
{
    return cocos2d::CC_CONTENT_SCALE_FACTOR();
}

void GAFAsset::setDesiredCsf(float csf)
{
    _desiredCsf = csf;
}

GAFAnimatedObject * GAFAsset::createObject()
{
    if (!m_currentTextureAtlas)
    {
        return nullptr;
    }

    return GAFAnimatedObject::create(this);
}

GAFAnimatedObject * GAFAsset::createObjectAndRun(bool looped)
{
    GAFAnimatedObject * res = createObject();
    if (res)
    {
        res->setLooped(looped);
        res->start();
    }
    return res;
}

GAFAsset::GAFAsset():
m_currentTextureAtlas(nullptr),
m_textureLoadDelegate(nullptr),
m_sceneFps(60),
m_sceneWidth(0),
m_sceneHeight(0)
{
    GAFShaderManager::Initialize();
}

GAFAsset::~GAFAsset()
{
    GAF_RELEASE_ARRAY(TextureAtlases_t, m_textureAtlases);
    GAF_RELEASE_ARRAY(AnimationFrames_t, m_animationFrames);
}

bool GAFAsset::isAssetVersionPlayable(const char * version)
{
    return true;
}

GAFAsset* GAFAsset::create(const std::string& gafFilePath, GAFTextureLoadDelegate* delegate)
{
    GAFAsset * ret = new GAFAsset();
    if (ret && ret->initWithGAFFile(gafFilePath, delegate))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}


GAFAsset* GAFAsset::createWithBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate* delegate /*= NULL*/)
{
    GAFAsset * ret = new GAFAsset();
    if (ret && ret->initWithGAFBundle(zipfilePath, entryFile, delegate))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}

bool GAFAsset::initWithGAFBundle(const std::string& zipFilePath, const std::string& entryFile, GAFTextureLoadDelegate* delegate /*= NULL*/)
{
    GAFLoader* loader = new GAFLoader();

    std::string fullfilePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(zipFilePath);

    cocos2d::ZipFile bundle(fullfilePath);
    ssize_t sz = 0;
    unsigned char* gafData = bundle.getFileData(entryFile, &sz);

    if (!gafData || !sz)
        return false;

    bool isLoaded = loader->loadData(gafData, sz, this);

    if (isLoaded)
    {
        _chooseTextureAtlas();

        if (m_currentTextureAtlas)
        {
            m_textureLoadDelegate = delegate;
            m_currentTextureAtlas->loadImages(entryFile, m_textureLoadDelegate, &bundle);
        }
    }

    delete loader;

    return isLoaded;
}

bool GAFAsset::initWithGAFFile(const std::string& filePath, GAFTextureLoadDelegate* delegate)
{
    GAFLoader* loader = new GAFLoader();

    std::string fullfilePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);

    bool isLoaded = loader->loadFile(fullfilePath, this);

    if (m_textureAtlases.empty())
    {
        return false;
    }

    if (isLoaded)
    {
        _chooseTextureAtlas();

        if (m_currentTextureAtlas)
        {
            m_textureLoadDelegate = delegate;
            m_currentTextureAtlas->loadImages(fullfilePath, m_textureLoadDelegate);
        }
    }

    delete loader;

    return isLoaded;
}

GAFTextureAtlas* GAFAsset::getTextureAtlas()
{
    return m_currentTextureAtlas;
}

size_t GAFAsset::getAnimationFramesCount() const
{
    return m_animationFrames.size();
}

const GAFAnimationSequence* GAFAsset::getSequence(const std::string& name) const
{
    AnimationSequences_t::const_iterator it = m_animationSequences.find(name);

    if (it != m_animationSequences.end())
    {
        return &it->second;
    }

    return nullptr;
}

const GAFAnimationSequence * GAFAsset::getSequenceByLastFrame(size_t frame) const
{
    if (m_animationSequences.empty())
    {
        return nullptr;
    }

    for (AnimationSequences_t::const_iterator i = m_animationSequences.begin(), e = m_animationSequences.end(); i != e; ++i)
    {
        if (i->second.endFrameNo == frame + 1)
        {
            return &i->second;
        }
    }

    return nullptr;
}

const GAFAnimationSequence * GAFAsset::getSequenceByFirstFrame(size_t frame) const
{
    if (m_animationSequences.empty())
    {
        return nullptr;
    }

    for (AnimationSequences_t::const_iterator i = m_animationSequences.begin(), e = m_animationSequences.end(); i != e; ++i)
    {
        if (i->second.startFrameNo == frame)
        {
            return &i->second;
        }
    }

    return nullptr;
}

void GAFAsset::pushTextureAtlas(GAFTextureAtlas* atlas)
{
    m_textureAtlases.push_back(atlas);
}

void GAFAsset::pushAnimationMask(unsigned int objectId, unsigned int elementAtlasIdRef)
{
    m_animationMasks[objectId] = elementAtlasIdRef;
}

void GAFAsset::pushAnimationObjects(unsigned int objectId, unsigned int elementAtlasIdRef)
{
    m_animationObjects[objectId] = elementAtlasIdRef;
}

void GAFAsset::pushAnimationFrame(GAFAnimationFrame* frame)
{
    m_animationFrames.push_back(frame);
}

const AnimationObjects_t& GAFAsset::getAnimationObjects() const
{
    return m_animationObjects;
}

const AnimationMasks_t& GAFAsset::getAnimationMasks() const
{
    return m_animationMasks;
}

const AnimationFrames_t& GAFAsset::getAnimationFrames() const
{
    return m_animationFrames;
}

void GAFAsset::pushAnimationSequence(const std::string nameId, int start, int end)
{
    GAFAnimationSequence seq;
    seq.name = nameId;
    seq.startFrameNo = start;
    seq.endFrameNo = end;

    m_animationSequences[nameId] = seq;
}

const AnimationSequences_t& GAFAsset::getAnimationSequences() const
{
    return m_animationSequences;
}

void GAFAsset::pushNamedPart(unsigned int objectIdRef, const std::string& name)
{
    m_namedParts[name] = objectIdRef;
}

const NamedParts_t& GAFAsset::getNamedParts() const
{
    return m_namedParts;
}

float GAFAsset::usedAtlasContentScaleFactor() const
{
    return _usedAtlasContentScaleFactor;
}

void GAFAsset::setHeader(GAFHeader& h)
{
    m_header = h;
}

void GAFAsset::setTextureLoadDelegate(GAFTextureLoadDelegate* delegate)
{
    m_textureLoadDelegate = delegate;
}

const GAFHeader& GAFAsset::getHeader() const
{
    return m_header;
}

const unsigned int GAFAsset::getSceneFps() const
{
    return m_sceneFps;
}

const unsigned int GAFAsset::getSceneWidth() const
{
    return m_sceneWidth;
}

const unsigned int GAFAsset::getSceneHeight() const
{
    return m_sceneHeight;
}

const cocos2d::Color4B& GAFAsset::getSceneColor() const
{
    return m_sceneColor;
}

void GAFAsset::setSceneFps(unsigned int value)
{
    m_sceneFps = value;
}

void GAFAsset::setSceneWidth(unsigned int value)
{
    m_sceneWidth = value;
}

void GAFAsset::setSceneHeight(unsigned int value)
{
    m_sceneHeight = value;
}

void GAFAsset::setSceneColor(const cocos2d::Color4B& value)
{
    m_sceneColor = value;
}

void GAFAsset::_chooseTextureAtlas()
{
    float atlasScale = m_textureAtlases[0]->getScale();

    m_currentTextureAtlas = m_textureAtlases[0];

    const unsigned int count = m_textureAtlases.size();

    for (unsigned int i = 1; i < count; ++i)
    {
        float as = m_textureAtlases[i]->getScale();
        if (fabs(atlasScale - _currentDeviceScale) > fabs(as - _currentDeviceScale))
        {
            m_currentTextureAtlas = m_textureAtlases[i];
            atlasScale = as;
        }
    }

    _usedAtlasContentScaleFactor = atlasScale;
}

NS_GAF_END
