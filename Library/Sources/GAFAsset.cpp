#include "GAFPrecompiled.h"
#include "GAFAsset.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFObject.h"
#include "GAFAssetTextureManager.h"
#include "GAFShaderManager.h"

#include "GAFLoader.h"

NS_GAF_BEGIN

static float  _desiredCsf = 1.f;

float GAFAsset::desiredAtlasScale()
{
    return m_desiredAtlasScale;
}

void GAFAsset::setDesiredAtlasScale(float scale)
{
    m_desiredAtlasScale = scale;
}

GAFObject * GAFAsset::createObject()
{
    if (m_timelines.empty())
    {
        return nullptr;
    }

    if (m_rootTimeline == nullptr)
    {
        CCLOG("%s", "You haven't root timeline in this asset. Please set root timeline by setRootTimeline(...)");
        for (Timelines_t::iterator i = m_timelines.begin(), e = m_timelines.end(); i != e; i++)
        {
            if (!i->second->getLinkageName().empty())
            {
                setRootTimeline(i->second);
                break;
            }
        }
    }

    return GAFObject::create(this, m_rootTimeline);
}

GAFObject* GAFAsset::createObjectAndRun(bool looped)
{
    GAFObject* res = createObject();
    if (res)
    {
        res->setLooped(looped, true);
        res->start();
    }
    return res;
}

GAFAsset::GAFAsset() :
m_textureLoadDelegate(nullptr),
m_sceneFps(60),
m_sceneWidth(0),
m_sceneHeight(0),
m_rootTimeline(nullptr),
m_desiredAtlasScale(1.0f),
m_gafFileName("")
{
    m_textureManager = new GAFAssetTextureManager();
    GAFShaderManager::Initialize();
}

GAFAsset::~GAFAsset()
{
    GAF_RELEASE_MAP(Timelines_t, m_timelines);
    //CC_SAFE_RELEASE(m_rootTimeline);
    m_textureManager->release();
}

bool GAFAsset::isAssetVersionPlayable(const char * version)
{
    (void)version;
    return true;
}

GAFAsset* GAFAsset::create(const std::string& gafFilePath, GAFTextureLoadDelegate_t delegate)
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

GAFAsset* GAFAsset::create(const std::string& gafFilePath)
{
    return create(gafFilePath, nullptr);
}

GAFAsset* GAFAsset::createWithBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate_t delegate /*= NULL*/)
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

bool GAFAsset::initWithGAFBundle(const std::string& zipFilePath, const std::string& entryFile, GAFTextureLoadDelegate_t delegate /*= NULL*/)
{
    GAFLoader* loader = new GAFLoader();

    m_gafFileName = zipFilePath;
    m_gafFileName.append("/" + entryFile);
    std::string fullfilePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(zipFilePath);

    cocos2d::ZipFile bundle(fullfilePath);
    ssize_t sz = 0;
    unsigned char* gafData = bundle.getFileData(entryFile, &sz);

    bool isLoaded = false;

    if (gafData && sz)
    {
        isLoaded = loader->loadData(gafData, sz, this);
    }
    if (isLoaded)
    {
        loadTextures(entryFile, delegate, &bundle);
    }

    delete loader;

    return isLoaded;
}

bool GAFAsset::initWithGAFFile(const std::string& filePath, GAFTextureLoadDelegate_t delegate)
{
    GAFLoader* loader = new GAFLoader();

    m_gafFileName = filePath;
    std::string fullfilePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);

    bool isLoaded = loader->loadFile(fullfilePath, this);

    if (m_timelines.empty())
    {
        delete loader;
        return false;
    }
    if (isLoaded)
    {
        loadTextures(fullfilePath, delegate);
    }

    delete loader;

    return isLoaded;
}

void GAFAsset::loadTextures(const std::string& filePath, GAFTextureLoadDelegate_t delegate, cocos2d::ZipFile* bundle /*= nullptr*/)
{
    for (Timelines_t::iterator i = m_timelines.begin(), e = m_timelines.end(); i != e; i++)
    {
        i->second->loadImages(m_desiredAtlasScale);

        if (i->second->getTextureAtlas())
        {
            m_textureManager->appendInfoFromTextureAtlas(i->second->getTextureAtlas());
            //i->second->getTextureAtlas()->loadImages(fullfilePath, m_textureLoadDelegate);
        }
    }

    m_textureLoadDelegate = delegate;
    m_textureManager->loadImages(filePath, m_textureLoadDelegate, bundle);
}

/*GAFTextureAtlas* GAFAsset::getTextureAtlas()
{
return m_currentTextureAtlas;
}*/

void GAFAsset::setRootTimeline(GAFTimeline *tl)
{
    m_rootTimeline = tl;
    //m_rootTimeline->retain();
    m_header.pivot = tl->getPivot();
    m_header.frameSize = tl->getRect();
}

bool GAFAsset::setRootTimeline(const std::string& name)
{
    for (Timelines_t::iterator i = m_timelines.begin(), e = m_timelines.end(); i != e; i++)
    {
        std::string tl_name = i->second->getLinkageName();
        if (tl_name.compare(name) == 0)
        {
            setRootTimeline(i->second);
            return true;
        }
    }
    return false;
}

bool GAFAsset::setRootTimeline(uint32_t id)
{
    Timelines_t::iterator timeline = m_timelines.find(id);
    if (timeline != m_timelines.end())
    {
        setRootTimeline(timeline->second);
        return true;
    }
    return false;
}

GAFTimeline* GAFAsset::getRootTimeline() const
{
    return m_rootTimeline;
}

GAFTimeline* GAFAsset::getTimelineByName(const std::string& name) const
{
    for (Timelines_t::value_type it : m_timelines)
    {
        if (it.second->getLinkageName() == name)
            return it.second;
    }

    return nullptr;
}

void GAFAsset::pushTimeline(uint32_t timelineIdRef, GAFTimeline* t)
{
    m_timelines[timelineIdRef] = t;
    t->retain();
}

void GAFAsset::setHeader(GAFHeader& h)
{
    m_header = h;
}

void GAFAsset::setTextureLoadDelegate(GAFTextureLoadDelegate_t delegate)
{
    m_textureLoadDelegate = delegate;
}

GAFAssetTextureManager* GAFAsset::getTextureManager()
{
    return m_textureManager;
}

Timelines_t& GAFAsset::getTimelines()
{
    return m_timelines;
}

const Timelines_t& GAFAsset::getTimelines() const
{
    return m_timelines;
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

const std::string& GAFAsset::getGAFFileName() const
{
    return m_gafFileName;
}

NS_GAF_END
