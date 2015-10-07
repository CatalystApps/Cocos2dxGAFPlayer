#include "GAFPrecompiled.h"
#include "GAFAsset.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFTextData.h"
#include "GAFObject.h"
#include "GAFAssetTextureManager.h"
#include "GAFShaderManager.h"
#include "GAFTimelineAction.h"

#include "GAFLoader.h"

#include "json/document.h"

NS_GAF_BEGIN

//static float  _desiredCsf = 1.f;

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

GAFAsset::GAFAsset() 
: m_textureLoadDelegate(nullptr)
, m_soundDelegate(nullptr)
, m_sceneFps(60)
, m_sceneWidth(0)
, m_sceneHeight(0)
, m_rootTimeline(nullptr)
, m_desiredAtlasScale(1.0f)
, m_gafFileName("")
, m_state(State::Normal)
{
}

GAFAsset::~GAFAsset()
{
    GAF_RELEASE_MAP(Timelines_t, m_timelines);
    GAF_RELEASE_MAP(SoundInfos_t, m_soundInfos);
    GAF_RELEASE_ARRAY(TextureAtlases_t, m_textureAtlases);
    //CC_SAFE_RELEASE(m_rootTimeline);
    if (m_state == State::Normal)
    {
        m_textureManager->release();
    }
}

bool GAFAsset::isAssetVersionPlayable(const char * version)
{
    (void)version;
    return true;
}

GAFAsset* GAFAsset::create(const std::string& gafFilePath, GAFTextureLoadDelegate_t delegate, GAFLoader* customLoader /*= nullptr*/)
{
    GAFAsset * ret = new GAFAsset();
    if (ret && ret->initWithGAFFile(gafFilePath, delegate, customLoader))
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

GAFAsset* GAFAsset::createWithBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate_t delegate, GAFLoader* customLoader /*= nullptr*/)
{
    GAFAsset * ret = new GAFAsset();
    if (ret && ret->initWithGAFBundle(zipfilePath, entryFile, delegate, customLoader))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}

GAFAsset* GAFAsset::createWithBundle(const std::string& zipfilePath, const std::string& entryFile)
{
    return createWithBundle(zipfilePath, entryFile, nullptr);
}

void GAFAsset::getResourceReferences(const std::string& gafFilePath, std::vector<GAFResourcesInfo*> &dest)
{
    GAFAsset * asset = new GAFAsset();
    asset->m_state = State::DryRun;
    if (asset && asset->initWithGAFFile(gafFilePath, nullptr))
    {
        asset->parseReferences(dest);
    }
    CC_SAFE_RELEASE(asset);
    return;
}

void GAFAsset::getResourceReferencesFromBundle(const std::string& zipfilePath, const std::string& entryFile, std::vector<GAFResourcesInfo*>& dest)
{
    GAFAsset * asset = new GAFAsset();
    asset->m_state = State::DryRun;
    if (asset && asset->initWithGAFBundle(zipfilePath, entryFile, nullptr))
    {
        asset->parseReferences(dest);
    }
    CC_SAFE_RELEASE(asset);
    return;
}

bool GAFAsset::initWithGAFBundle(const std::string& zipFilePath, const std::string& entryFile, GAFTextureLoadDelegate_t delegate, GAFLoader* customLoader /*= nullptr*/)
{
    m_gafFileName = zipFilePath;
    m_gafFileName.append("/" + entryFile);
    std::string fullfilePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(zipFilePath);

    cocos2d::ZipFile bundle(fullfilePath);
    ssize_t sz = 0;
    unsigned char* gafData = bundle.getFileData(entryFile, &sz);

    bool isLoaded = false;

    if (gafData && sz)
    {
        if (customLoader)
        {
            customLoader->loadData(gafData, sz, this);
        }
        else
        {
            GAFLoader* loader = new GAFLoader();
            isLoaded = loader->loadData(gafData, sz, this);
            delete loader;
        }
    }
    if (isLoaded && m_state == State::Normal)
    {
        m_textureManager = new GAFAssetTextureManager();
        GAFShaderManager::Initialize();
        loadTextures(entryFile, delegate, &bundle);
    }

    return isLoaded;
}

bool GAFAsset::initWithGAFFile(const std::string& filePath, GAFTextureLoadDelegate_t delegate, GAFLoader* customLoader /*= nullptr*/)
{
    m_gafFileName = filePath;
    std::string fullfilePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);

    bool isLoaded = false;
    if (customLoader)
    {
        isLoaded = customLoader->loadFile(fullfilePath, this);
    }
    else
    {
        GAFLoader* loader = new GAFLoader();
        isLoaded = loader->loadFile(fullfilePath, this);
        delete loader;
    }

    if (m_timelines.empty())
    {
        return false;
    }
    if (isLoaded && m_state == State::Normal)
    {
        m_textureManager = new GAFAssetTextureManager();
        GAFShaderManager::Initialize();
        loadTextures(fullfilePath, delegate);
    }

    return isLoaded;
}

void GAFAsset::parseReferences(std::vector<GAFResourcesInfo*>& dest)
{
    for (auto i = m_timelines.begin(), e = m_timelines.end(); i != e; ++i)
    {
        // textures
        auto atlases = i->second->getTextureAtlases();
        for (auto i_atlas = atlases.begin(), e_atlas = atlases.end(); i_atlas != e_atlas; ++i_atlas)
        {
            auto atlasInfos = (*i_atlas)->getAtlasInfos();
            for (auto i_info = atlasInfos.begin(), e_info = atlasInfos.end(); i_info != e_info; ++i_info)
            {
                for (uint32_t j = 0; j < (*i_info).m_sources.size(); ++j)
                {
                    auto& aiSource = (*i_info).m_sources[j];
                    
                    GAFResourcesInfoTexture compareTexture(aiSource.source, aiSource.csf);
                    // check duplicates
                    bool present = false;
                    for (auto i_res = dest.begin(), e_res = dest.end(); i_res != e_res; ++i_res)
                    {
                        if ((*i_res)->id == GAFResourcesInfo::ResourceId::Texture)
                        {
                            GAFResourcesInfoTexture *presentTexture = reinterpret_cast<GAFResourcesInfoTexture*>(*i_res);
                            
                            if (*presentTexture == compareTexture)
                            {
                                present = true;
                                break;
                            }
                        }
                    }

                    if (!present)
                    {
                        dest.push_back(new GAFResourcesInfoTexture(compareTexture));
                    }
                }
            }
        }

        // fonts
        auto textDatas = i->second->getTextsData();
        for (auto i_text = textDatas.begin(), e_text = textDatas.end(); i_text != e_text; ++i_text)
        {
            GAFResourcesInfoFont compareFont(i_text->second->m_textFormat.m_font);
            // check duplicates
            bool present = false;
            for (auto i_res = dest.begin(), e_res = dest.end(); i_res != e_res; ++i_res)
            {
                if ((*i_res)->id == GAFResourcesInfo::ResourceId::Font)
                {
                    GAFResourcesInfoFont *presentFont = reinterpret_cast<GAFResourcesInfoFont*>(*i_res);

                    if (*presentFont == compareFont)
                    {
                        present = true;
                        break;
                    }
                }
            }

            if (!present)
            {
                dest.push_back(new GAFResourcesInfoFont(compareFont));
            }
        }

        // sounds - TODO
    }
}

void GAFAsset::loadTextures(const std::string& filePath, GAFTextureLoadDelegate_t delegate, cocos2d::ZipFile* bundle /*= nullptr*/)
{
    for (Timelines_t::iterator i = m_timelines.begin(), e = m_timelines.end(); i != e; i++)
    {
        i->second->loadImages(m_desiredAtlasScale);

        if (i->second->getTextureAtlas())
        {
            m_textureManager->appendInfoFromTextureAtlas(i->second->getTextureAtlas());
        }
    }

    loadImages(m_desiredAtlasScale);
    if (getTextureAtlas())
    {
        m_textureManager->appendInfoFromTextureAtlas(getTextureAtlas());
    }

    m_textureLoadDelegate = delegate;
    m_textureManager->loadImages(filePath, m_textureLoadDelegate, bundle);
}

void GAFAsset::loadImages(float desiredAtlasScale)
{
    if (m_textureAtlases.empty())
    {
        m_currentTextureAtlas = nullptr;
        return;
    }
    _chooseTextureAtlas(desiredAtlasScale);
}

void GAFAsset::_chooseTextureAtlas(float desiredAtlasScale)
{
    float atlasScale = m_textureAtlases[0]->getScale();

    m_currentTextureAtlas = m_textureAtlases[0];

    const size_t count = m_textureAtlases.size();

    for (size_t i = 1; i < count; ++i)
    {
        float as = m_textureAtlases[i]->getScale();
        if (fabs(atlasScale - desiredAtlasScale) > fabs(as - desiredAtlasScale))
        {
            m_currentTextureAtlas = m_textureAtlases[i];
            atlasScale = as;
        }
    }
}

GAFSprite* GAFAsset::getCustomRegion(const std::string& name)
{
    GAFTextureAtlas* atlas = getTextureAtlas();
    const GAFTextureAtlas::Elements_t& elementsMap = atlas->getElements();
    cocos2d::SpriteFrame * spriteFrame = nullptr;

    GAFTextureAtlas::Elements_t::const_iterator elIt = std::find_if(
        elementsMap.begin(),
        elementsMap.end(),
        [name](const std::pair<uint32_t, GAFTextureAtlasElement*>& pair) { return pair.second->name == name; }); // Search for atlas element by its xref
    
    assert(elIt != elementsMap.end());
    const GAFTextureAtlasElement* txElemet = nullptr;
    if (elIt != elementsMap.end())
    {
        txElemet = elIt->second;
        GAFAssetTextureManager* txMgr = getTextureManager();
        cocos2d::Texture2D * texture = txMgr->getTextureById(txElemet->atlasIdx + 1);
        if (texture)
        {
            spriteFrame = cocos2d::SpriteFrame::createWithTexture(texture, txElemet->bounds);
        }
        else
        {
            CCLOGERROR("Cannot add sub object with name: %s, atlas with idx: %d not found.", name.c_str(), txElemet->atlasIdx);
        }
    }

    GAFSprite* result = nullptr;
    if (spriteFrame)
    {
        result = new GAFSprite();
        
        result->initWithSpriteFrame(spriteFrame, txElemet->rotation);
        cocos2d::Vec2 pt = cocos2d::Vec2(0 - (0 - (txElemet->pivotPoint.x / result->getContentSize().width)),
            0 + (1 - (txElemet->pivotPoint.y / result->getContentSize().height)));
        result->setAnchorPoint(pt);

        if (txElemet->getScale() != 1.0f)
        {
            result->setAtlasScale(1.0f / txElemet->getScale());
        }
        result->setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);
    }

    return result;
}

void GAFAsset::useExternalTextureAtlas(std::vector<cocos2d::Texture2D *> &textures, GAFTextureAtlas::Elements_t& elements)
{
    for (size_t i = 0, e = textures.size(); i < e; i++)
    {
        m_textureManager->swapTexture(static_cast<uint32_t>(i+1), textures[i]);
    }
    
    for (Timelines_t::iterator i = m_timelines.begin(), e = m_timelines.end(); i != e; ++i)
    {
        for (auto& element : elements)
        {
            i->second->getTextureAtlas()->swapElement(element.first, element.second);
        }
    }
}

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

void GAFAsset::pushSound(uint32_t id, GAFSoundInfo* sound)
{
    m_soundInfos[id] = sound;
}

void GAFAsset::pushTextureAtlas(GAFTextureAtlas* atlas)
{
    m_textureAtlases.push_back(atlas);
}

void GAFAsset::soundEvent(GAFTimelineAction *action)
{
    if (!m_soundDelegate) return;

    std::string soundParams = action->getParam(GAFTimelineAction::PI_EVENT_DATA);

    uint32_t soundId;
    GAFSoundInfo::SyncEvent syncEvent;
    int32_t repeat = 1;

    rapidjson::Document doc;
    doc.Parse<0>(soundParams.c_str());

    const rapidjson::Value& jsonSoundId = doc["id"];
    soundId = jsonSoundId.GetInt();

    const rapidjson::Value& jsonSyncEvent = doc["action"];
    uint32_t tmpSyncEvent = jsonSyncEvent.GetInt();
    syncEvent = static_cast<GAFSoundInfo::SyncEvent>(tmpSyncEvent);

    if (doc.HasMember("repeat"))
    {
        const rapidjson::Value& jsonRepeat = doc["repeat"];
        repeat = jsonRepeat.GetInt();
        if (repeat == 0) repeat = 1;
    }

    SoundInfos_t::iterator it = m_soundInfos.find(soundId);
    CC_ASSERT(it != m_soundInfos.end());

    m_soundDelegate(it->second, repeat, syncEvent);
}

void GAFAsset::setHeader(GAFHeader& h)
{
    m_header = h;
}

void GAFAsset::setTextureLoadDelegate(GAFTextureLoadDelegate_t delegate)
{
    m_textureLoadDelegate = delegate;
}

void GAFAsset::setSoundDelegate(GAFSoundDelegate_t delegate)
{
    m_soundDelegate = delegate;
}

GAFAssetTextureManager* GAFAsset::getTextureManager()
{
    return m_textureManager;
}

GAFTextureAtlas* GAFAsset::getTextureAtlas()
{
    return m_currentTextureAtlas;
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
