#include "GAFPrecompiled.h"
#include "GAFObject.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"
#include "GAFTextureAtlas.h"
#include "GAFAssetTextureManager.h"
#include "GAFTextureAtlasElement.h"
#include "GAFMovieClip.h"
#include "GAFMask.h"
#include "GAFAnimationFrame.h"
#include "GAFSubobjectState.h"
#include "GAFFilterData.h"
#include "GAFTextField.h"

#include <math/TransformUtils.h>

#define ENABLE_RUNTIME_FILTERS 1

NS_GAF_BEGIN

cocos2d::AffineTransform GAFObject::GAF_CGAffineTransformCocosFormatFromFlashFormat(cocos2d::AffineTransform aTransform)
{
    cocos2d::AffineTransform transform = aTransform;
    transform.b = -transform.b;
    transform.c = -transform.c;
    transform.ty = - transform.ty;
    return transform;
}


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
    m_charType = GAFCharacterType::Timeline;
	m_parentColorTransforms[0] = cocos2d::Vec4::ONE;
	m_parentColorTransforms[1] = cocos2d::Vec4::ZERO;
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

    GAF_SAFE_RELEASE_ARRAY_WITH_NULL_CHECK(DisplayList_t, m_displayList);

    m_fps = m_asset->getSceneFps();

    m_animationsSelectorScheduled = false;
    
    instantiateObject(m_timeline->getAnimationObjects(), m_timeline->getAnimationMasks());
}

void GAFObject::instantiateObject(const AnimationObjects_t& objs, const AnimationMasks_t& masks)
{
	uint32_t maxIdx = 0;
	for (AnimationObjects_t::const_iterator it = objs.begin(), e = objs.end(); it != e; ++it)
	{
		if (it->first > maxIdx)
		{
			maxIdx = it->first;
		}
	}
	for (AnimationMasks_t::const_iterator it = masks.begin(), e = masks.end(); it != e; ++it)
	{
		if (it->first > maxIdx)
		{
			maxIdx = it->first;
		}
	}

    instantiateAnimatedObjects(objs, maxIdx);
    instantiateMasks(masks);
}

void GAFObject::instantiateAnimatedObjects(const AnimationObjects_t &objs, int max)
{
	m_displayList.resize(max + 1);

    for (AnimationObjects_t::const_iterator i = objs.begin(), e = objs.end(); i != e; ++i)
    {
        GAFCharacterType charType = std::get<1>(i->second);
        uint32_t reference = std::get<0>(i->second);
        uint32_t objectId = i->first;

        if (charType == GAFCharacterType::Timeline)
        {
            encloseNewTimeline(reference, objectId);
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
                m_displayList[objectId] = sprite;

            }
            else
            {
                assert(false);
                CCLOGERROR("Cannot add subnode with AtlasElementRef: %d, not found in atlas(es). Ignoring.", atlasElementIdRef);
            }
        }
        else if (charType == GAFCharacterType::TextField)
        {
            TextsData_t::const_iterator it = m_timeline->getTextsData().find(reference);
            if (it != m_timeline->getTextsData().end())
            {
                GAFTextField *tf = new GAFTextField();
                tf->initWithTextData(it->second);
                m_displayList[objectId] = tf;
            }
        }
    }
}

void GAFObject::encloseNewTimeline(uint32_t reference, uint32_t objId)
{
    Timelines_t& timelines = m_asset->getTimelines();

    CCAssert(reference != IDNONE, "Invalid object reference.");

    Timelines_t::iterator tl = timelines.find(reference);

    CCAssert(tl != timelines.end(), "Invalid object reference.");

    GAFObject* newObject = GAFObject::create(m_asset, tl->second);
    m_displayList[objId] = newObject;
    
    
    newObject->retain();
    addChild(newObject);

    if (!newObject->getIsAnimationRunning())
    {
        newObject->m_currentFrame = GAFFirstFrameIndex;
        newObject->setAnimationRunning(true);
    }
}

void GAFObject::instantiateMasks(const AnimationMasks_t& masks)
{
    uint32_t maxIdx = 0;
    for (AnimationMasks_t::const_iterator it = masks.begin(), e = masks.end(); it != e; ++it)
    {
        if (it->first > maxIdx)
        {
            maxIdx = it->first;
        }
    }

    m_masksDList.resize(maxIdx + 1);

    for (AnimationMasks_t::const_iterator i = masks.begin(), e = masks.end(); i != e; ++i)
    {
        GAFCharacterType charType = std::get<1>(i->second);
        uint32_t reference = std::get<0>(i->second);
        uint32_t objectId = i->first;

        if (charType == GAFCharacterType::Timeline)
        {
            encloseNewTimeline(reference, objectId);
        }
        else if (charType == GAFCharacterType::Texture)
        {
            GAFTextureAtlas* atlas = m_timeline->getTextureAtlas();
            const GAFTextureAtlas::Elements_t& elementsMap = atlas->getElements();

            unsigned int atlasElementIdRef = std::get<0>(i->second);

            GAFTextureAtlas::Elements_t::const_iterator elIt = elementsMap.find(atlasElementIdRef); // Search for atlas element by it's xref

            assert(elIt != elementsMap.end());

            const GAFTextureAtlasElement* txElemet = nullptr;
            cocos2d::SpriteFrame * spriteFrame = nullptr;

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

                if (spriteFrame)
                {
                    GAFMask *mask = new GAFMask(m_stencilLayer);
                    mask->initWithSpriteFrame(spriteFrame);

                    mask->objectIdRef = i->first;
                    cocos2d::Vect pt = cocos2d::Vect(0 - (0 - (txElemet->pivotPoint.x / mask->getContentSize().width)),
                                                     0 + (1 - (txElemet->pivotPoint.y / mask->getContentSize().height)));

                    mask->setAnchorPoint(pt);
                    if (txElemet->scale != 1.0f)
                    {
                        mask->setAtlasScale(1.0f / txElemet->scale);
                    }

                    m_masksDList[objectId] = mask;
                    //addChild(mask);
                }
            }
        }
    }
}

void GAFObject::updateStencilLayer(int newLayer)
{
    m_stencilLayer = newLayer;

    for (DisplayList_t::iterator it = m_masksDList.begin(), e = m_masksDList.end(); it != e; ++it)
    {
        if (*it == nullptr)
        {
            continue;
        }

        GAFMask* mask = static_cast<GAFMask*>(*it);
        mask->updateStencilLayer(newLayer);
    }
}

void GAFObject::processAnimation()
{
    realizeFrame(this, m_currentFrame);
}

void GAFObject::setAnimationRunning(bool value)
{
    m_isRunning = value;

    for (auto obj : m_displayList)
    {
        if (obj == nullptr)
        {
            continue;
        }
        obj->setAnimationRunning(value);
    }
}

bool GAFObject::getIsAnimationRunning() const
{
    return m_isRunning;
}

void GAFObject::setSequenceDelegate(GAFSequenceDelegate_t delegate)
{
    m_sequenceDelegate = delegate;
}

void GAFObject::setAnimationFinishedPlayDelegate(GAFAnimationFinishedPlayDelegate_t delegate)
{
    m_animationFinishedPlayDelegate = delegate;
}

void GAFObject::setAnimationStartedNextLoopDelegate(GAFAnimationStartedNextLoopDelegate_t delegate)
{
    m_animationStartedNextLoopDelegate = delegate;
}

void GAFObject::setFramePlayedDelegate(GAFFramePlayedDelegate_t delegate)
{
    m_framePlayedDelegate = delegate;
}

void GAFObject::setControlDelegate(GAFObjectControlDelegate_t delegate)
{
    m_controlDelegate = delegate;
}

void GAFObject::start()
{
    schedule(cocos2d::SEL_SCHEDULE(&GAFObject::processAnimations));

    m_animationsSelectorScheduled = true;
    
    if (!m_isRunning)
    {
        m_currentFrame = GAFFirstFrameIndex;
        setAnimationRunning(true);
    }
}

void GAFObject::stop()
{
    unschedule(cocos2d::SEL_SCHEDULE(&GAFObject::processAnimations));
    m_animationsSelectorScheduled = false;
    
    if (m_isRunning)
    {
        m_currentFrame = GAFFirstFrameIndex;
        setAnimationRunning(false);
    }
}

void GAFObject::processAnimations(float dt)
{
    m_timeDelta += dt;
    double frameTime = 1.0 / m_fps;
    while (m_timeDelta >= frameTime)
    {
        m_timeDelta -= frameTime;
        step();
        
        if (m_framePlayedDelegate)
        {
            m_framePlayedDelegate(this, m_currentFrame);
        }
    }
}

void GAFObject::pauseAnimation()
{
    if (m_isRunning)
    {
        setAnimationRunning(false);
    }
}

void GAFObject::resumeAnimation()
{
    if (!m_isRunning)
    {
        setAnimationRunning(true);
    }
}

bool GAFObject::isDone() const
{
    if (m_isLooped)
    {
        return false;
    }
    else
    {
        if (!m_isReversed)
        {
            return m_currentFrame > m_totalFrameCount;
        }
        else
        {
            return m_currentFrame < GAFFirstFrameIndex - 1;
        }
    }
}

bool GAFObject::isLooped() const
{
    return m_isLooped;
}

void GAFObject::setLooped(bool looped)
{
    m_isLooped = looped;

    for (auto obj : m_displayList)
    {
        if (obj == nullptr)
        {
            continue;
        }
        obj->setLooped(looped);
    }
}

bool GAFObject::isReversed() const
{
    return m_isReversed;
}

void GAFObject::setReversed(bool reversed)
{
    m_isReversed = reversed;

    for (auto obj : m_displayList)
    {
        if (obj == nullptr)
        {
            continue;
        }
        obj->setLooped(reversed);
    }
}

uint32_t GAFObject::getTotalFrameCount() const
{
    return m_totalFrameCount;
}

uint32_t GAFObject::getCurrentFrameIndex() const
{
    return m_currentFrame;
}

bool GAFObject::setFrame(uint32_t index)
{
    if (index < m_totalFrameCount)
    {
        m_currentFrame = index;
        processAnimation();
        return true;
    }
    return false;
}

bool GAFObject::gotoAndStop(const std::string& frameLabel)
{
    uint32_t f = getStartFrame(frameLabel);
    if (IDNONE == f)
    {
        return false;
    }
    return gotoAndStop(f);
}

bool GAFObject::gotoAndStop(uint32_t frameNumber)
{
    if (setFrame(frameNumber))
    {
        setAnimationRunning(false);
        return true;
    }
    return false;
}

bool GAFObject::gotoAndPlay(const std::string& frameLabel)
{
    uint32_t f = getStartFrame(frameLabel);
    if (IDNONE == f)
    {
        return false;
    }
    return gotoAndPlay(f);
}

bool GAFObject::gotoAndPlay(uint32_t frameNumber)
{
    if (setFrame(frameNumber))
    {
        setAnimationRunning(true);
        return true;
    }
    return false;
}

uint32_t GAFObject::getStartFrame(const std::string& frameLabel)
{
    if (!m_asset)
    {
        return IDNONE;
    }

    const GAFAnimationSequence * seq = m_timeline->getSequence(frameLabel);
    if (seq)
    {
        return seq->startFrameNo;
    }
    return IDNONE;

}

uint32_t GAFObject::getEndFrame(const std::string& frameLabel)
{
    if (!m_asset)
    {
        return IDNONE;
    }
    const GAFAnimationSequence * seq = m_timeline->getSequence(frameLabel);
    if (seq)
    {
        return seq->endFrameNo;
    }
    return IDNONE;
}

bool GAFObject::playSequence(const std::string& name, bool looped /*= false*/, bool resume /*= true*/)
{
    if (!m_asset || !m_timeline)
    {
        return false;
    }

    if (name.empty())
    {
        return false;
    }

    uint32_t s = getStartFrame(name);
    uint32_t e = getEndFrame(name);

    if (IDNONE == s || IDNONE == e)
    {
        return false;
    }

    m_currentSequenceStart = s;
    m_currentSequenceEnd = e;

    if (m_currentFrame < m_currentSequenceStart || m_currentFrame > m_currentSequenceEnd)
    {
        m_currentFrame = m_currentSequenceStart;
    }
    else
    {
        m_currentFrame = m_currentSequenceStart;
    }

    setLooped(looped);

    if (resume)
    {
        resumeAnimation();
    }
    else
    {
        stop();
    }

    return true;
}

void GAFObject::clearSequence()
{
    m_currentSequenceStart = GAFFirstFrameIndex;
    m_currentSequenceEnd = m_totalFrameCount;
}

void GAFObject::step()
{
    if (!m_isReversed)
    {
        if (m_currentFrame < m_currentSequenceStart)
        {
            m_currentFrame = m_currentSequenceStart;
        }

        if (m_sequenceDelegate && m_timeline)
        {
            const GAFAnimationSequence * seq = m_timeline->getSequenceByLastFrame(m_currentFrame);
            if (seq)
            {
                m_sequenceDelegate(this, seq->name);
            }
        }

        if (m_currentFrame >= m_currentSequenceEnd)
        {
            if (m_isLooped)
            {
                m_currentFrame = m_currentSequenceStart;

                if (m_animationStartedNextLoopDelegate)
                {
                    m_animationStartedNextLoopDelegate(this);
                }
            }
            else
            {
                setAnimationRunning(false);

                if (m_animationFinishedPlayDelegate)
                {
                    m_animationFinishedPlayDelegate(this);
                }

                return;
            }
        }

        processAnimation();

        if (getIsAnimationRunning())
        {
            ++m_currentFrame;
        }
    }
    else
    {
        // If switched to reverse after final frame played
        if (m_currentFrame >= m_currentSequenceEnd)
        {
            m_currentFrame = m_currentSequenceEnd - 1;
        }

        if (m_sequenceDelegate && m_timeline)
        {
            const GAFAnimationSequence * seq = m_timeline->getSequenceByFirstFrame(m_currentFrame + 1);
            if (seq)
            {
                m_sequenceDelegate(this, seq->name);
            }
        }

        if (m_currentFrame < m_currentSequenceStart)
        {
            if (m_isLooped)
            {
                m_currentFrame = m_currentSequenceEnd - 1;

                if (m_animationStartedNextLoopDelegate)
                {
                    m_animationStartedNextLoopDelegate(this);
                }
            }
            else
            {
                setAnimationRunning(false);

                if (m_animationFinishedPlayDelegate)
                {
                    m_animationFinishedPlayDelegate(this);
                }

                return;
            }
        }

        processAnimation();

        if (getIsAnimationRunning())
        {
            --m_currentFrame;
        }
    }
}

bool GAFObject::hasSequences() const
{
    return m_timeline->getAnimationSequences().empty();
}

static cocos2d::Rect GAFCCRectUnion(const cocos2d::Rect& src1, const cocos2d::Rect& src2)
{
    float thisLeftX = src1.origin.x;
    float thisRightX = src1.origin.x + src1.size.width;
    float thisTopY = src1.origin.y + src1.size.height;
    float thisBottomY = src1.origin.y;

    if (thisRightX < thisLeftX)
    {
        std::swap(thisRightX, thisLeftX); // This rect has negative width
    }

    if (thisTopY < thisBottomY)
    {
        std::swap(thisTopY, thisBottomY); // This rect has negative height
    }

    float otherLeftX = src2.origin.x;
    float otherRightX = src2.origin.x + src2.size.width;
    float otherTopY = src2.origin.y + src2.size.height;
    float otherBottomY = src2.origin.y;

    if (otherRightX < otherLeftX)
    {
        std::swap(otherRightX, otherLeftX); // Other rect has negative width
    }

    if (otherTopY < otherBottomY)
    {
        std::swap(otherTopY, otherBottomY); // Other rect has negative height
    }

    float combinedLeftX = std::min(thisLeftX, otherLeftX);
    float combinedRightX = std::max(thisRightX, otherRightX);
    float combinedTopY = std::max(thisTopY, otherTopY);
    float combinedBottomY = std::min(thisBottomY, otherBottomY);

    return cocos2d::Rect(combinedLeftX, combinedBottomY, combinedRightX - combinedLeftX, combinedTopY - combinedBottomY);
}

cocos2d::Rect GAFObject::getBoundingBoxForCurrentFrame()
{
    cocos2d::Rect result = cocos2d::Rect::ZERO;

    bool isFirstObj = true;
    for (DisplayList_t::iterator i = m_displayList.begin(), e = m_displayList.end(); i != e; ++i)
    {
        if (*i == nullptr)
        {
            continue;
        }

        GAFSprite* anim = *i;
        if (anim->isVisible())
        {
            cocos2d::Rect bb = anim->getBoundingBox();
            if (isFirstObj)
                result = bb;
            else
                result = GAFCCRectUnion(result, bb);
        }

        isFirstObj = false;
    }

    return cocos2d::RectApplyTransform(result, getNodeToParentTransform());
}

cocos2d::Mat4 const& GAFObject::getNodeToParentTransform() const
{
    if (m_charType == GAFCharacterType::Timeline)
        return cocos2d::Node::getNodeToParentTransform();
    else
        return GAFSprite::getNodeToParentTransform();
}

void GAFObject::realizeFrame(cocos2d::Node* out, size_t frameIndex)
{
    const AnimationFrames_t& animationFrames = m_timeline->getAnimationFrames();
    if (m_parentColorTransforms[0].x < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    if (animationFrames.size() > frameIndex)
    {
        GAFAnimationFrame *currentFrame = animationFrames[frameIndex];

        for (SpriteList_t::iterator i = m_visibleObjects.begin(), e = m_visibleObjects.end(); i != e; ++i)
        {
            (*i)->setVisible(false);
        }

        m_visibleObjects.clear();

        const GAFAnimationFrame::SubobjectStates_t& states = currentFrame->getObjectStates();

        for (const GAFSubobjectState* state : states)
        {
            GAFObject* subObject = m_displayList[state->objectIdRef];

            if (subObject != nullptr)
            {
                if (subObject->m_charType == GAFCharacterType::Timeline)
                {
                    cocos2d::AffineTransform stateTransform = state->affineTransform;
                    float csf = m_timeline->usedAtlasContentScaleFactor();
                    stateTransform.tx *= csf;
                    stateTransform.ty *= csf;
                    cocos2d::AffineTransform t = GAF_CGAffineTransformCocosFormatFromFlashFormat(state->affineTransform);
                    subObject->setAdditionalTransform(t);
                    subObject->m_parentFilters.clear();
                    const Filters_t& filters = state->getFilters();
                    subObject->m_parentFilters.insert(subObject->m_parentFilters.end(), filters.begin(), filters.end());

                    const float* cm = state->colorMults();
                    subObject->m_parentColorTransforms[0] = cocos2d::Vec4(
                        m_parentColorTransforms[0].x * cm[0],
                        m_parentColorTransforms[0].y * cm[1],
                        m_parentColorTransforms[0].z * cm[2],
                        m_parentColorTransforms[0].w * cm[3]);
                    subObject->m_parentColorTransforms[1] = cocos2d::Vec4(state->colorOffsets()) + m_parentColorTransforms[1];

                    subObject->setLocalZOrder(state->zIndex);

                    subObject->step();

                }
                else if (subObject->m_charType == GAFCharacterType::Texture)
                {
                    cocos2d::Vect prevAP = subObject->getAnchorPoint();
                    cocos2d::Size  prevCS = subObject->getContentSize();

#if ENABLE_RUNTIME_FILTERS
                    // Validate sprite type (w/ or w/o filter)
                    const Filters_t& filters = state->getFilters();
                    GAFFilterData* filter = NULL;

                    GAFMovieClip* mc = static_cast<GAFMovieClip*>(subObject);

                    Filters_t filtersUnion;
                    filtersUnion.insert(filtersUnion.end(), m_parentFilters.begin(), m_parentFilters.end());
                    filtersUnion.insert(filtersUnion.end(), filters.begin(), filters.end());

                    if (!filtersUnion.empty())
                    {
                        filter = filtersUnion[0];
                        filter->apply(mc);
                    }

                    if (!filter || filter->getType() != GAFFilterType::GFT_Blur)
                    {
                        mc->setBlurFilterData(nullptr);
                    }

                    if (!filter || filter->getType() != GAFFilterType::GFT_ColorMatrix)
                    {
                        mc->setColorMarixFilterData(nullptr);
                    }

                    if (!filter || filter->getType() != GAFFilterType::GFT_Glow)
                    {
                        mc->setGlowFilterData(nullptr);
                    }

                    if (!filter || filter->getType() != GAFFilterType::GFT_DropShadow)
                    {
                        GAFDropShadowFilterData::reset(mc);
                    }
#endif

                    cocos2d::Size newCS = subObject->getContentSize();
                    cocos2d::Vect newAP = cocos2d::Vect(((prevAP.x - 0.5f) * prevCS.width) / newCS.width + 0.5f,
                        ((prevAP.y - 0.5f) * prevCS.height) / newCS.height + 0.5f);
                    subObject->setAnchorPoint(newAP);

                    if (state->maskObjectIdRef == IDNONE)
                    {
                        if (!subObject->getParent())
                        {
                            out->addChild(subObject);
                        }
                    }
                    else
                    {
                        if (subObject->getParent())
                        {
                            out->removeChild(subObject, false);
                        }

                        if (!m_masksDList.empty())
                        {
                            GAFMask* mask = static_cast<GAFMask*>(m_masksDList[state->maskObjectIdRef]);

                            mask->addMaskedObject(subObject);

                            if (mask->getParent() != this)
                            {
                                addChild(mask);
                            }
                        }
                    }

                    cocos2d::AffineTransform stateTransform = state->affineTransform;
                    float csf = m_timeline->usedAtlasContentScaleFactor();
                    stateTransform.tx *= csf;
                    stateTransform.ty *= csf;
                    cocos2d::AffineTransform t = GAF_CGAffineTransformCocosFormatFromFlashFormat(state->affineTransform);
                    subObject->setExternaTransform(t);
                    if (subObject->getLocalZOrder() != state->zIndex)
                    {
                        subObject->setLocalZOrder(state->zIndex);
                    }
                    subObject->setVisible(state->isVisible());
                    m_visibleObjects.push_back(subObject);

                    cocos2d::Vec4 cm = cocos2d::Vec4(state->colorMults());
                    float colorMults[4] = { 
                        state->colorMults()[0] * m_parentColorTransforms[0].x,
                        state->colorMults()[1] * m_parentColorTransforms[0].y,
                        state->colorMults()[2] * m_parentColorTransforms[0].z,
                        state->colorMults()[3] * m_parentColorTransforms[0].w
                    };
                    float colorOffsets[4] = {
                        state->colorOffsets()[0] * m_parentColorTransforms[1].x,
                        state->colorOffsets()[1] * m_parentColorTransforms[1].y,
                        state->colorOffsets()[2] * m_parentColorTransforms[1].z,
                        state->colorOffsets()[3] * m_parentColorTransforms[1].w
                    };
                    
                    mc->setColorTransform(colorMults, colorOffsets);
                }
                else if (subObject->m_charType == GAFCharacterType::TextField)
                {
                    GAFTextField *tf = static_cast<GAFTextField*>(subObject);

                    if (subObject->getParent())
                    {
                        out->removeChild(subObject, false);
                    }

                    addChild(tf);
                }
            }
            else if (m_masksDList.size() > state->objectIdRef)
            {
                // TODO: mask-timeline
                GAFObject* mask = m_masksDList[state->objectIdRef];
                if (mask != nullptr)
                {
                    mask->setExternaTransform(GAF_CGAffineTransformCocosFormatFromFlashFormat(state->affineTransform));

                    if (mask->getLocalZOrder() != state->zIndex)
                    {
                        mask->setLocalZOrder(state->zIndex);
                    }
                }
            }
        }

        GAFAnimationFrame::TimelineActions_t timelineActions = currentFrame->getTimelineActions();
        for (GAFTimelineAction action : timelineActions)
        {
            switch (action.getType())
            {
            case GAFActionType::Stop:
                stop();
                break;
            case GAFActionType::Play:
                start();
                break;
            case GAFActionType::GotoAndStop: 
                gotoAndStop(action.getParam(GAFTimelineAction::PI_FRAME));
                break;
            case GAFActionType::GotoAndPlay:
				gotoAndPlay(action.getParam(GAFTimelineAction::PI_FRAME));
            case GAFActionType::DispatchEvent:
				_eventDispatcher->dispatchCustomEvent(action.getParam(GAFTimelineAction::PI_EVENT_TYPE), &action);
				break;

            case GAFActionType::None:
            default:
                break;
            }
        }
    }


}

void GAFObject::setStencilLayer(int newLayer)
{
    m_stencilLayer = std::max(-1, std::min(newLayer, 255));
    updateStencilLayer(m_stencilLayer);
}

void GAFObject::incStencilLayer()
{
    m_stencilLayer = std::max(-1, std::min(++m_stencilLayer, 255));
    updateStencilLayer(m_stencilLayer);
}

void GAFObject::decStencilLayer()
{
    m_stencilLayer = std::max(-1, std::min(--m_stencilLayer, 255));
    updateStencilLayer(m_stencilLayer);
}

int GAFObject::getStencilLayer() const
{
    return m_stencilLayer;
}

uint32_t GAFObject::getFps() const
{
    return m_fps;
}

void GAFObject::setFps(uint32_t value)
{
    CCASSERT(value, "Error! Fps is set to zero.");
    m_fps = value;
}

GAFObject* GAFObject::getObjectByName(const std::string& name)
{
    if (!name.empty())
    {
        std::stringstream ss(name);
        std::string item;
        typedef std::vector<std::string> StringVec_t;
        StringVec_t elems;
        while (std::getline(ss, item, '.'))
        {
            elems.push_back(item);
        }

        GAFObject* retval = nullptr;

        if (!elems.empty())
        {
            const NamedParts_t& np = m_timeline->getNamedParts();
            NamedParts_t::const_iterator it = np.find(elems[0]);

            if (it != np.end())
            {
                retval = m_displayList[it->second];

                if (elems.size() > 1)
                {
                    StringVec_t::iterator begIt = elems.begin() + 1;

                    while (begIt != elems.end())
                    {
                        const NamedParts_t& np = retval->m_timeline->getNamedParts();
                        NamedParts_t::const_iterator it = np.find(*begIt);
                        if (it != np.end())
                        {
                            retval = retval->m_displayList[it->second];
                        }
                        else
                        {
                            // Sequence is incorrect
                            //break;

                            // It is better to return nil instead of the last found object in a chain
                            return nullptr;
                        }

                        ++begIt;
                    }
                }

                return retval;
            }
        }
    }

    return nullptr;
}

const GAFObject* GAFObject::getObjectByName(const std::string& name) const
{
    return const_cast<GAFObject*>(this)->getObjectByName(name);
}

NS_GAF_END