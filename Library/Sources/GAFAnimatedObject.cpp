#include "GAFPrecompiled.h"
#include "GAFAsset.h"
#include "GAFAnimatedObject.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFAnimationFrame.h"
#include "GAFSubobjectState.h"
#include "GAFSpriteWithAlpha.h"
#include "GAFStencilMaskSprite.h"
#include "GAFFilterData.h"

// Detect whether it is Visual Studio 2010 or lower
#if defined(_MSC_VER) && _MSC_VER < 1700 
// If so, implement roundf manually
static inline float roundf(float x)
    {
        return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
    }
#endif

static unsigned long ccNextPOT(unsigned long x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;
}

#define DEBUG_RTT_DRAWING 0

static CCAffineTransform GAF_CGAffineTransformCocosFormatFromFlashFormat(CCAffineTransform aTransform)
{
    CCAffineTransform transform = aTransform;
    transform.b = -transform.b;
    transform.c = -transform.c;
    transform.ty = -transform.ty;
    return transform;
}

GAFAnimatedObject::GAFAnimatedObject()
:
_asset(NULL),
_extraFramesCounter(0),
_framePlayedDelegate(NULL),
_controlDelegate(NULL),
m_stencilLayer(-1),
m_batch(NULL)
{
}

GAFAnimatedObject::~GAFAnimatedObject()
{
    CC_SAFE_RELEASE(_asset);

    GAF_SAFE_RELEASE_MAP(SubObjects_t, m_subObjects);

    GAF_SAFE_RELEASE_MAP(SubObjects_t, m_masks);
}

GAFAnimatedObject * GAFAnimatedObject::create(GAFAsset * anAsset)
{
    GAFAnimatedObject * ret = new GAFAnimatedObject();
    if (ret && ret->init(anAsset))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return NULL;
}

GAFAnimatedObject * GAFAnimatedObject::createAndRun(const std::string& gafPath, bool looped)
{
    GAFAsset * asset = GAFAsset::create(gafPath);
    return asset->createObjectAndRun(looped);
}

bool GAFAnimatedObject::init(GAFAsset * anAsset)
{
    if (!anAsset)
    {
        return false;
    }

    if (!GAFAnimation::init(anAsset))
    {
        return false;
    }

    if (_asset != anAsset)
    {
        CC_SAFE_RELEASE(_asset);
        _asset = anAsset;
        CC_SAFE_RETAIN(_asset);
    }

    GAF_SAFE_RELEASE_MAP(SubObjects_t, m_subObjects);
    GAF_SAFE_RELEASE_MAP(SubObjects_t, m_masks);

    _FPSType = kGAFAnimationFPSType_60;
    m_fps = anAsset->getSceneFps();
    _extraFramesCounter = 0;
    _animationsSelectorScheduled = false;

    instantiateObject(_asset->getAnimationObjects(), _asset->getAnimationMasks());

    return true;
}

unsigned int GAFAnimatedObject::objectIdByObjectName(const std::string& aName)
{
    const NamedParts_t& np = _asset->getNamedParts();

    NamedParts_t::const_iterator it = np.find(aName);

    if (it != np.end())
    {
        return it->second;
    }

    return IDNONE;
}

void GAFAnimatedObject::instantiateObject(const AnimationObjects_t& objs, const AnimationMasks_t& masks)
{
    for (AnimationObjects_t::const_iterator i = objs.begin(), e = objs.end(); i != e; ++i)
    {
        GAFTextureAtlas* atlas = _asset->textureAtlas();
        const GAFTextureAtlas::Elements_t& elementsMap = atlas->getElements();
        CCSpriteFrame * spriteFrame = NULL;

        unsigned int atlasElementIdRef = i->second;

        GAFTextureAtlas::Elements_t::const_iterator elIt = elementsMap.find(atlasElementIdRef); // Search for atlas element by its xref

        assert(elIt != elementsMap.end());

        const GAFTextureAtlasElement* txElemet = NULL;

        if (elIt != elementsMap.end())
        {
            txElemet = elIt->second;

            if (atlas->textures()->count() >= txElemet->atlasIdx + 1)
            {
                CCTexture2D * texture = (CCTexture2D *)atlas->textures()->objectAtIndex(txElemet->atlasIdx);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, txElemet->bounds);
            }
            else
            {
                CCLOGERROR("Cannot add sub object with Id: %d, atlas with idx: %d not found.", atlasElementIdRef, txElemet->atlasIdx);
            }
        }

        if (spriteFrame && txElemet)
        {
            GAFSpriteWithAlpha *sprite = new GAFSpriteWithAlpha();
            sprite->initWithSpriteFrame(spriteFrame);

            sprite->objectIdRef = i->first;

            sprite->setVisible(false);
            CCPoint pt = CCPointMake(0 - (0 - (txElemet->pivotPoint.x / sprite->getContentSize().width)),
                0 + (1 - (txElemet->pivotPoint.y / sprite->getContentSize().height)));
            sprite->setAnchorPoint(pt);

            if (txElemet->scale != 1.0f)
            {
                sprite->setAtlasScale(1.0f / txElemet->scale);
            }
            // visual studio compile fix
            ccBlendFunc blend = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
            sprite->setBlendFunc(blend);
            m_subObjects[i->first] = sprite;
        }
        else
        {
            assert(false);
            CCLOGERROR("Cannot add subnode with AtlasElementRef: %d, not found in atlas(es). Ignoring.", atlasElementIdRef);
        }

    }

    for (AnimationMasks_t::const_iterator i = masks.begin(), e = masks.end(); i != e; ++i)
    {
        GAFTextureAtlas* atlas = _asset->textureAtlas();
        const GAFTextureAtlas::Elements_t& elementsMap = atlas->getElements();

        unsigned int atlasElementIdRef = i->second;

        GAFTextureAtlas::Elements_t::const_iterator elIt = elementsMap.find(atlasElementIdRef); // Search for atlas element by it's xref

        assert(elIt != elementsMap.end());

        const GAFTextureAtlasElement* txElemet = NULL;
        CCSpriteFrame * spriteFrame = NULL;

        if (elIt != elementsMap.end())
        {
            txElemet = elIt->second;

            if (atlas->textures()->count() >= txElemet->atlasIdx + 1)
            {
                CCTexture2D * texture = (CCTexture2D *)atlas->textures()->objectAtIndex(txElemet->atlasIdx);
                spriteFrame = CCSpriteFrame::createWithTexture(texture, txElemet->bounds);
            }
            else
            {
                CCLOGERROR("Cannot add sub object with Id: %d, atlas with idx: %d not found.", atlasElementIdRef, txElemet->atlasIdx);
            }

            if (spriteFrame)
            {
                GAFStencilMaskSprite *mask = new GAFStencilMaskSprite(m_stencilLayer);
                mask->initWithSpriteFrame(spriteFrame);
                mask->objectIdRef = i->first;
                CCPoint pt = CCPointMake(0 - (0 - (txElemet->pivotPoint.x / mask->getContentSize().width)),
                    0 + (1 - (txElemet->pivotPoint.y / mask->getContentSize().height)));

                mask->setAnchorPoint(pt);
                if (txElemet->scale != 1.0f)
                {
                    mask->setAtlasScale(1.0f / txElemet->scale);
                }

                m_masks[i->first] = mask;
                //addChild(mask);
            }
        }
    }
}

bool GAFAnimatedObject::captureControlOverSubobjectNamed(const char * aName, GAFAnimatedObjectControlFlags aControlFlags)
{
    unsigned int objectId = objectIdByObjectName(aName);
    if (IDNONE == objectId)
    {
        return false;
    }

    CaptureObjects_t::const_iterator cpoIt = m_capturedObjects.find(objectId);

    if (cpoIt != m_capturedObjects.end())
    {
        return false;
    }

    m_capturedObjects[objectId] = aControlFlags;
    return true;
}

void GAFAnimatedObject::releaseControlOverSubobjectNamed(const char * aName)
{
    unsigned int objectId = objectIdByObjectName(aName);
    if (objectId != IDNONE)
    {
        CaptureObjects_t::iterator cpoIt = m_capturedObjects.find(objectId);

        if (cpoIt != m_capturedObjects.end())
        {
            m_capturedObjects.erase(cpoIt);
        }
    }
}

void GAFAnimatedObject::removeAllSubObjects()
{
    for (SubObjects_t::iterator i = m_subObjects.begin(), e = m_subObjects.end(); i != e; ++i)
    {
        GAFSprite* sprite = i->second;
        sprite->setVisible(false);
        sprite->removeFromParentAndCleanup(true);
    }

    GAF_SAFE_RELEASE_MAP(SubObjects_t, m_subObjects);
}

void GAFAnimatedObject::setSubobjectsVisible(bool visible)
{
    for (SubObjects_t::iterator i = m_subObjects.begin(), e = m_subObjects.end(); i != e; ++i)
    {
        GAFSprite* sprite = i->second;
        sprite->setVisible(visible);
    }
}

void GAFAnimatedObject::processAnimations(float dt)
{
    if (++_extraFramesCounter >= numberOfGlobalFramesForOneAnimationFrame())
    {
        _extraFramesCounter = 0;

        if (isAnimationRunning())
        {
            step();

            if (_framePlayedDelegate)
            {
                _framePlayedDelegate->onFramePlayed(this, currentFrameIndex());
            }
        }
    }
}

CCPoint GAFAnimatedObject::pupilCoordinatesWithXSemiaxis(float anXSemiaxis, float anYSemiaxis, CCPoint aCenter, CCPoint anExternalPoint)
{
    CCPoint rePoint = CCPointZero;
    float const firstConst = (anYSemiaxis * anYSemiaxis);
    float const secondConst = firstConst / (anXSemiaxis * anXSemiaxis);
    float length = std::numeric_limits<float>::max();
    float tmpLenght = length;
    float xStart = -anXSemiaxis;
    float xEnd = 0;

    if (((anExternalPoint.x - aCenter.x) * (anExternalPoint.x - aCenter.x)) / (anXSemiaxis * anXSemiaxis) +
        ((anExternalPoint.y - aCenter.y) * (anExternalPoint.y - aCenter.y)) / (anYSemiaxis * anYSemiaxis) < (1.0 + FLT_EPSILON))
    {
        return anExternalPoint;
    }
    if (anExternalPoint.x > aCenter.x)
    {
        xStart = 0;
        xEnd = anXSemiaxis;
    }
    for (float i = xStart; i <= xEnd; i += 0.5f)
    {
        float y = sqrtf(firstConst - secondConst * (i * i));
        if (anExternalPoint.y > aCenter.y)
        {
            y = -y;
        }
        float dX = anExternalPoint.x - (aCenter.x + i);
        float dY = anExternalPoint.y - (aCenter.y - y);
        tmpLenght = sqrtf(dX * dX + dY * dY);
        if (tmpLenght < length)
        {
            length = tmpLenght;
            rePoint = CCPointMake(aCenter.x + i, aCenter.y - y);
        }
    }
    return rePoint;
}

GAFSprite * GAFAnimatedObject::subObjectForInnerObjectId(unsigned int anInnerObjectId)
{
    for (SubObjects_t::iterator i = m_subObjects.begin(), e = m_subObjects.end(); i != e; ++i)
    {
        GAFSprite* anim = i->second;
        if (anim->objectIdRef == anInnerObjectId)
        {
            return anim;
        }
    }
    return NULL;
}

const SubObjects_t& GAFAnimatedObject::getSubojects() const
{
    return m_subObjects;
}

const SubObjects_t& GAFAnimatedObject::getMasks() const
{
    return m_masks;
}

void GAFAnimatedObject::start()
{
    GAFAnimation::start();

    schedule(SEL_SCHEDULE(&GAFAnimatedObject::processAnimations));
    _animationsSelectorScheduled = true;
}

void GAFAnimatedObject::stop()
{
    GAFAnimation::stop();

    unschedule(SEL_SCHEDULE(&GAFAnimatedObject::processAnimations));
    _animationsSelectorScheduled = false;
}

int GAFAnimatedObject::numberOfGlobalFramesForOneAnimationFrame()
{
    float globalFPS = roundf(1.0f / (float)CCDirector::sharedDirector()->getAnimationInterval());

    if (globalFPS > (float)m_fps - FLT_EPSILON)
    {
        return (int)roundf(globalFPS / (float)m_fps);
    }
    else
    {
        return 1;
    }
}

CCSprite* GAFAnimatedObject::renderCurrentFrameToTexture(bool usePOTTextures)
{
    CCRect frameRect = realBoundingBoxForCurrentFrame();

    if (frameRect.size.width == 0.f || frameRect.size.height == 0.f)
    {
        return NULL;
    }

    CCPoint originalPos = getPosition();

    int width = 0;
    int height = 0;

    if (usePOTTextures)
    {
        width = (int)ccNextPOT((int)frameRect.size.width);
        height = (int)ccNextPOT((int)frameRect.size.height);
    }
    else
    {
        width = (int)frameRect.size.width;
        height = (int)frameRect.size.height;
    }

    CCPoint transformedPosition = CCPointApplyAffineTransform(frameRect.origin, nodeToParentTransform());

    CCRenderTexture* rt = CCRenderTexture::create(width, height, kCCTexture2DPixelFormat_RGBA8888, GL_DEPTH24_STENCIL8);

    setPosition(ccp(originalPos.x - transformedPosition.x, originalPos.y - transformedPosition.y));

#if DEBUG_RTT_DRAWING
    rt->beginWithClear(0, 0, 0, 0);
#else
    rt->begin();
#endif

    visit();

    rt->end();

    setPosition(originalPos);

    CCSprite* res = CCSprite::createWithTexture(rt->getSprite()->getTexture());
    res->setAnchorPoint(ccp(0, 0));

    res->setPosition(transformedPosition);
    return(res);
}

void GAFAnimatedObject::realizeFrame(CCNode* out, int frameIndex)
{
    GAFAnimationFrame *currentFrame = _asset->getAnimationFrames()[frameIndex];
    setSubobjectsVisible(false);
    
    const GAFAnimationFrame::SubobjectStates_t& states = currentFrame->getObjectStates();
    {
        size_t statesCount = states.size();
        for (size_t i = 0; i < statesCount; ++i)
        {
            const GAFSubobjectState *state = states[i];
            {
                GAFSpriteWithAlpha *subObject = NULL;

                SubObjects_t::iterator sboIt = m_subObjects.find(state->objectIdRef);

                if (sboIt != m_subObjects.end())
                {
                    subObject = static_cast<GAFSpriteWithAlpha*>(sboIt->second);
                }

                if (subObject)
                {
                    // Validate sprite type (w/ or w/o filter)
                    const Filters_t& filters = state->getFilters();
                    GAFFilterData* filter = NULL;

                    if (!filters.empty())
                    {
                        filter = filters[0];
                        filter->apply(subObject);
                    }

                    CCPoint prevAP = subObject->getAnchorPoint();
                    CCSize  prevCS = subObject->getContentSize();

                    if (!filter || filter->getType() != GFT_Blur)
                    {
                        subObject->setBlurFilterData(NULL);
                    }

                    if (!filter || filter->getType() != GFT_ColorMatrix)
                    {
                        subObject->setColorMarixFilterData(NULL);
                    }

                    if (!filter || filter->getType() != GFT_Glow)
                    {
                        subObject->setGlowFilterData(NULL);
                    }

                    if (!filter || filter->getType() != GFT_DropShadow)
                    {
                        GAFDropShadowFilterData::reset(subObject);
                    }

                    CCSize newCS = subObject->getContentSize();
                    CCPoint newAP = CCPointMake(((prevAP.x - 0.5) * prevCS.width) / newCS.width + 0.5,
                        ((prevAP.y - 0.5) * prevCS.height) / newCS.height + 0.5);
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
                        GAFStencilMaskSprite * mask = NULL;

                        if (!m_masks.empty())
                        {
                            mask = static_cast<GAFStencilMaskSprite *>(m_masks[state->maskObjectIdRef]);
                        }

                        if (mask)
                        {
                            mask->addMaskedObject(subObject);

                            if (mask->getParent() != this)
                            {
                                addChild(mask);
                            }
                        }
                    }

                    bool subobjectCaptured = false;
                    GAFAnimatedObjectControlFlags controlFlags = kGAFAnimatedObjectControl_None;

                    CaptureObjects_t::const_iterator cpoIt = m_capturedObjects.find(state->objectIdRef);
                    if (cpoIt != m_capturedObjects.end())
                    {
                        subobjectCaptured = true;
                        controlFlags = (GAFAnimatedObjectControlFlags)cpoIt->second;
                    }

                    if (!subobjectCaptured ||
                        (subobjectCaptured && (controlFlags & kGAFAnimatedObjectControl_ApplyState)))
                    {
                        CCAffineTransform stateTransform = state->affineTransform;
                        float csf = _asset->usedAtlasContentScaleFactor();
                        stateTransform.tx *= csf;
                        stateTransform.ty *= csf;
                        CCAffineTransform t = GAF_CGAffineTransformCocosFormatFromFlashFormat(state->affineTransform);
                        subObject->setExternaTransform(t);
                        if (subObject->getZOrder() != state->zIndex)
                        {
                            subObject->setZOrder(state->zIndex);
                            GAFStencilMaskSprite::updateMaskContainerOf(subObject);
                        }
                        subObject->setVisible(state->isVisible());
                        subObject->setColorTransform(state->colorMults(), state->colorOffsets());
                    }
                }
                else
                {
                    GAFSprite * mask = NULL;
                    if (!m_masks.empty())
                    {
                        mask = m_masks[state->objectIdRef];
                    }

                    if (mask)
                    {
                        mask->setExternaTransform(GAF_CGAffineTransformCocosFormatFromFlashFormat(state->affineTransform));

                        if (mask->getZOrder() != state->zIndex)
                        {
                            mask->setZOrder(state->zIndex);
                            GAFStencilMaskSprite::updateMaskContainerOf(subObject);
                        }
                    }
                }
            }
        }
    }

    if (_controlDelegate)
    {
        size_t statesCount = states.size();
        for (size_t i = 0; i < statesCount; ++i)
        {
            const GAFSubobjectState *state = states[i];

            SubObjects_t::iterator sboIt = m_subObjects.find(state->objectIdRef);

            if (sboIt != m_subObjects.end())
            {
                const GAFSpriteWithAlpha *subObject = static_cast<const GAFSpriteWithAlpha*>(m_subObjects[state->objectIdRef]);

                CaptureObjects_t::const_iterator cpoIt = m_capturedObjects.find(state->objectIdRef);

                bool subobjectCaptured = cpoIt != m_capturedObjects.end();
                if (subobjectCaptured && _controlDelegate)
                {
                    _controlDelegate->onFrameDisplayed(this, subObject);
                }
            }
            else
            {
                // Masks cannot be captured right now
            }
        }
    }
}

void GAFAnimatedObject::processAnimation()
{
    CCNode* baseParent = this;

    if (m_batch)
    {
        baseParent = m_batch;
    }

    realizeFrame(baseParent, _currentFrameIndex);
}

void GAFAnimatedObject::setFramePlayedDelegate(GAFFramePlayedDelegate * delegate)
{
    _framePlayedDelegate = delegate;
}

void GAFAnimatedObject::setControlDelegate(GAFAnimatedObjectControlDelegate * delegate)
{
    _controlDelegate = delegate;
}

static CCRect GAFCCRectUnion(const CCRect& src1, const CCRect& src2)
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

    return CCRect(combinedLeftX, combinedBottomY, combinedRightX - combinedLeftX, combinedTopY - combinedBottomY);
}

CCRect GAFAnimatedObject::realBoundingBoxForCurrentFrame()
{
    CCRect result = CCRectZero;

    for (SubObjects_t::iterator i = m_subObjects.begin(), e = m_subObjects.end(); i != e; ++i)
    {
        GAFSprite* anim = i->second;
        if (anim->isVisible())
        {
            CCRect bb = anim->boundingBox();
            result = GAFCCRectUnion(result, bb);
        }
    }

    return result;
}

void GAFAnimatedObject::draw()
{
    CCLayer::draw();
#if DEBUG_RTT_DRAWING
    ccDrawCircle(getAnchorPointInPoints(), 5, 0, 8, false);
#endif
}

void GAFAnimatedObject::setStencilLayer(int newLayer)
{
    m_stencilLayer = std::max(-1, std::min(newLayer, 255));
    _updateStencilLayer(m_stencilLayer);
}

void GAFAnimatedObject::incStencilLayer()
{
    m_stencilLayer = std::max(-1, std::min(++m_stencilLayer, 255));
    _updateStencilLayer(m_stencilLayer);
}

void GAFAnimatedObject::decStencilLayer()
{
    m_stencilLayer = std::max(-1, std::min(--m_stencilLayer, 255));
    _updateStencilLayer(m_stencilLayer);
}

int GAFAnimatedObject::getStencilLayer() const
{
    return m_stencilLayer;
}

void GAFAnimatedObject::enableBatching(bool value)
{
    if (value && !m_batch)
    {
        GAFTextureAtlas* atlas = _asset->textureAtlas();

        if (atlas->textures()->count() == 1 && _asset->getAnimationMasks().empty())
        {
            CCTexture2D * texture = (CCTexture2D *)atlas->textures()->objectAtIndex(0);
            m_batch = new CCSpriteBatchNode();
            assert(m_batch->initWithTexture(texture, _asset->getAnimationObjects().size()));
            addChild(m_batch);
        }
    }
    else if (!value && m_batch)
    {
        m_batch->removeFromParent();
    }
}

void GAFAnimatedObject::_updateStencilLayer(int newLayer)
{
    m_stencilLayer = newLayer;

    for (SubObjects_t::iterator it = m_masks.begin(), e = m_masks.end(); it != e; ++it)
    {
        GAFStencilMaskSprite* mask = static_cast<GAFStencilMaskSprite*>(it->second);
        mask->updateStencilLayer(newLayer);
    }
}

