#include "GAFPrecompiled.h"
#include "GAFObject.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"
#include "GAFTextureAtlas.h"
#include "GAFAssetTextureManager.h"
#include "GAFTextureAtlasElement.h"
#include "GAFMovieClip.h"


GAFObject::GAFObject():
m_totalFrameCount(0),
m_currentSequenceStart(0),
m_currentSequenceEnd(0),
m_isRunning(false),
m_isLooped(false),
m_isReversed(false),
m_timeDelta(0.0),
m_fps(0),
m_stencilLayer(-1),
m_asset(nullptr),
m_timeline(nullptr),
m_currentFrame(GAFFirstFrameIndex),
m_animationsSelectorScheduled(false)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA

    static bool invalidateGLPrograms = false;

    auto listenerFG = cocos2d::EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, [this](cocos2d::EventCustom* event)
    {
        (void)event;
        if (invalidateGLPrograms)
        {
            cocos2d::ShaderCache::getInstance()->addGLProgram(nullptr, kGAFSpriteWithAlphaShaderProgramCache_noCTX);
            cocos2d::ShaderCache::getInstance()->addGLProgram(nullptr, kGAFSpriteWithAlphaShaderProgramCacheKey);
            cocos2d::ShaderCache::getInstance()->addGLProgram(nullptr, kGAFStencilMaskAlphaFilterProgramCacheKey);

            invalidateGLPrograms = false;
        }

        this->removeAllChildrenWithCleanup(true);
        this->_constructObject();
    });

    auto listenerBG = cocos2d::EventListenerCustom::create(EVENT_COME_TO_BACKGROUND, [this](cocos2d::EventCustom* event)
    {
        (void)event;
        invalidateGLPrograms = true;
    });

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerFG, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerBG, this);

#endif

}

GAFObject::~GAFObject()
{

}

GAFObject * GAFObject::create(GAFAsset * anAsset, GAFTimeline* timeline)
{
    GAFObject* ret = new GAFObject();

    if (ret && ret->init(anAsset, timeline))
    {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_RELEASE(ret);
    return nullptr;
}

bool GAFObject::init(GAFAsset * anAnimationData, GAFTimeline* timeline)
{
    CCAssert(anAnimationData, "anAssetData data should not be nil");
    CCAssert(timeline, "Timeline data should not be nil");

    if (!anAnimationData || !timeline)
    {
        return false;
    }

    if (m_asset != anAnimationData)
    {
        CC_SAFE_RELEASE(m_asset);
        m_asset = anAnimationData;
        CC_SAFE_RETAIN(m_asset);
    }

    if (m_timeline != timeline)
    {
        CC_SAFE_RELEASE(m_timeline);
        m_timeline = timeline;
        CC_SAFE_RETAIN(m_timeline);
    }

    m_currentSequenceStart = m_currentFrame = GAFFirstFrameIndex;

    m_currentSequenceEnd = m_totalFrameCount = m_timeline->getFramesCount();
    
    constructObject();

    return true;
}

void GAFObject::constructObject()
{
    cocos2d::Rect size = m_asset->getHeader().frameSize;

    setContentSize(cocos2d::Size(size.size.width + size.origin.x * 2, size.size.height + size.origin.y * 2));

    GAF_SAFE_RELEASE_MAP(DisplayList_t, m_displayList);

    m_fps = m_asset->getSceneFps();

    m_animationsSelectorScheduled = false;
}

void GAFObject::instantiateObject(const AnimationObjects_t& objs, const AnimationMasks_t& masks)
{
    for (AnimationObjects_t::const_iterator i = objs.begin(), e = objs.end(); i != e; ++i)
    {
        GAFCharacterType charType = std::get<1>(i->second);
        uint32_t reference = std::get<0>(i->second);
        uint32_t objectId = i->first;

        DisplayListObjectInfo di = { charType, objectId };

        if (charType == GAFCharacterType::Timeline)
        {
            Timelines_t& timelines = m_asset->getTimelines();

            CCAssert(reference != IDNONE, "Invalid object reference.");

            Timelines_t::iterator tl = timelines.find(reference);

            CCAssert(tl != timelines.end(), "Invalid object reference.");

            GAFObject* newObject = GAFObject::create(m_asset, tl->second);
            m_displayList[di] = newObject;
            addChild(newObject);
            newObject->start();
        }
        else if (charType == GAFCharacterType::Texture)
        {
            GAFTextureAtlas* atlas = m_timeline->getTextureAtlas();
            const GAFTextureAtlas::Elements_t& elementsMap = atlas->getElements();
            cocos2d::SpriteFrame * spriteFrame = nullptr;

            uint32_t atlasElementIdRef = std::get<0>(i->second);

            GAFTextureAtlas::Elements_t::const_iterator elIt = elementsMap.find(atlasElementIdRef); // Search for atlas element by its xref

            assert(elIt != elementsMap.end());

            const GAFTextureAtlasElement* txElemet = NULL;

            if (elIt != elementsMap.end())
            {
                txElemet = elIt->second;

                GAFAssetTextureManager* txMgr = m_asset->getTextureManager();

                cocos2d::Texture2D * texture = txMgr->getTextureById(txElemet->atlasIdx + 1);
                if (texture)
                {
                    spriteFrame = cocos2d::SpriteFrame::createWithTexture(texture, txElemet->bounds);
                }
                else
                {
                    CCLOGERROR("Cannot add sub object with Id: %d, atlas with idx: %d not found.", atlasElementIdRef, txElemet->atlasIdx);
                }
            }

            if (spriteFrame && txElemet)
            {
                GAFMovieClip *sprite = new GAFMovieClip();
                sprite->initWithSpriteFrame(spriteFrame);

                sprite->objectIdRef = objectId;

                sprite->setVisible(false);
                cocos2d::Vect pt = cocos2d::Vect(0 - (0 - (txElemet->pivotPoint.x / sprite->getContentSize().width)),
                                                 0 + (1 - (txElemet->pivotPoint.y / sprite->getContentSize().height)));
                sprite->setAnchorPoint(pt);

                if (txElemet->scale != 1.0f)
                {
                    sprite->setAtlasScale(1.0f / txElemet->scale);
                }
                // visual studio compile fix
                sprite->setBlendFunc({ GL_ONE, GL_ONE_MINUS_SRC_ALPHA });
                m_displayList[di] = sprite;

            }
            else
            {
                assert(false);
                CCLOGERROR("Cannot add subnode with AtlasElementRef: %d, not found in atlas(es). Ignoring.", atlasElementIdRef);
            }
        }

    }
}
