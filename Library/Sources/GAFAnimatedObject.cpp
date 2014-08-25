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

// Still is under development
#define ENABLE_RUNTIME_FILTERS 1

// Detect whether it is Visual Studio 2010 or lower
#if (defined(_MSC_VER) && _MSC_VER < 1700) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
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

static cocos2d::AffineTransform GAF_CGAffineTransformCocosFormatFromFlashFormat(cocos2d::AffineTransform aTransform)
{
    cocos2d::AffineTransform transform = aTransform;
    transform.b = -transform.b;
    transform.c = -transform.c;
    transform.ty = -transform.ty;
    return transform;
}

GAFAnimatedObject::GAFAnimatedObject()
:
m_asset(NULL),
m_stencilLayer(-1),
m_framePlayedDelegate(NULL),
m_controlDelegate(NULL),
m_extraFramesCounter(0),
m_timeDelta(0.f)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    
    static bool invalidateGLPrograms = false;
    
    auto listenerFG = cocos2d::EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, [this](cocos2d::EventCustom* event)
    {
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
        invalidateGLPrograms = true;
    });

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerFG, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerBG, this);

#endif
}

GAFAnimatedObject::~GAFAnimatedObject()
{
    CC_SAFE_RELEASE(m_asset);

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

    if (m_asset != anAsset)
    {
        CC_SAFE_RELEASE(m_asset);
        m_asset = anAsset;
        CC_SAFE_RETAIN(m_asset);
    }
    
    _constructObject();

    return true;
}

void GAFAnimatedObject::_constructObject()
{
    cocos2d::Rect size = m_asset->getHeader().frameSize;
    
    setContentSize(cocos2d::Size(size.size.width + size.origin.x * 2, size.size.height + size.origin.y * 2));
    
    GAF_SAFE_RELEASE_MAP(SubObjects_t, m_subObjects);
    GAF_SAFE_RELEASE_MAP(SubObjects_t, m_masks);
    
    _FPSType = kGAFAnimationFPSType_60;
    m_fps = m_asset->getSceneFps();
    m_extraFramesCounter = 0;
    _animationsSelectorScheduled = false;
    
    instantiateObject(m_asset->getAnimationObjects(), m_asset->getAnimationMasks());

}

unsigned int GAFAnimatedObject::objectIdByObjectName(const std::string& aName)
{
    const NamedParts_t& np = m_asset->getNamedParts();

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
        GAFTextureAtlas* atlas = m_asset->getTextureAtlas();
        const GAFTextureAtlas::Elements_t& elementsMap = atlas->getElements();
        cocos2d::SpriteFrame * spriteFrame = NULL;

        unsigned int atlasElementIdRef = i->second;

        GAFTextureAtlas::Elements_t::const_iterator elIt = elementsMap.find(atlasElementIdRef); // Search for atlas element by its xref

        assert(elIt != elementsMap.end());

        const GAFTextureAtlasElement* txElemet = NULL;

        if (elIt != elementsMap.end())
        {
            txElemet = elIt->second;

            if ((unsigned int)atlas->textures()->count() >= txElemet->atlasIdx + 1)
            {
                cocos2d::Texture2D * texture = (cocos2d::Texture2D *)atlas->textures()->getObjectAtIndex(txElemet->atlasIdx);
                spriteFrame = cocos2d::SpriteFrame::createWithTexture(texture, txElemet->bounds);
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
            cocos2d::Vect pt = cocos2d::Vect(0 - (0 - (txElemet->pivotPoint.x / sprite->getContentSize().width)),
                0 + (1 - (txElemet->pivotPoint.y / sprite->getContentSize().height)));
            sprite->setAnchorPoint(pt);

            if (txElemet->scale != 1.0f)
            {
                sprite->setAtlasScale(1.0f / txElemet->scale);
            }
            // visual studio compile fix
            cocos2d::BlendFunc blend = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
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
        GAFTextureAtlas* atlas = m_asset->getTextureAtlas();
        const GAFTextureAtlas::Elements_t& elementsMap = atlas->getElements();

        unsigned int atlasElementIdRef = i->second;

        GAFTextureAtlas::Elements_t::const_iterator elIt = elementsMap.find(atlasElementIdRef); // Search for atlas element by it's xref

        assert(elIt != elementsMap.end());

        const GAFTextureAtlasElement* txElemet = NULL;
        cocos2d::SpriteFrame * spriteFrame = NULL;

        if (elIt != elementsMap.end())
        {
            txElemet = elIt->second;

            if (atlas->textures()->count() >= txElemet->atlasIdx + 1)
            {
                cocos2d::Texture2D * texture = (cocos2d::Texture2D *)atlas->textures()->getObjectAtIndex(txElemet->atlasIdx);
                spriteFrame = cocos2d::SpriteFrame::createWithTexture(texture, txElemet->bounds);
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
                cocos2d::Vect pt = cocos2d::Vect(0 - (0 - (txElemet->pivotPoint.x / mask->getContentSize().width)),
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
bool GAFAnimatedObject::captureControlOverSubobject(unsigned int id, GAFAnimatedObjectControlFlags aControlFlags)
{
    if (IDNONE == id)
    {
        return false;
    }

    CaptureObjects_t::const_iterator cpoIt = m_capturedObjects.find(id);

    if (cpoIt != m_capturedObjects.end())
    {
        return false;
    }

    m_capturedObjects[id] = aControlFlags;
    return true;
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
        CaptureObjects_t::const_iterator cpoIt = m_capturedObjects.find(objectId);

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
    if (!isAnimationRunning())
        return;

    m_timeDelta += dt;
    double frameTime = 1.0 / m_fps;
    while (m_timeDelta >= frameTime)
    {
        m_timeDelta -= frameTime;
        step();

        if (m_framePlayedDelegate)
        {
            m_framePlayedDelegate->onFramePlayed(this, currentFrameIndex());
        }
    }
}

cocos2d::Vect GAFAnimatedObject::pupilCoordinatesWithXSemiaxis(float anXSemiaxis, float anYSemiaxis, cocos2d::Vect aCenter, cocos2d::Vect anExternalPoint)
{
    cocos2d::Vect rePoint = cocos2d::Vect::ZERO;
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
            rePoint = cocos2d::Vect(aCenter.x + i, aCenter.y - y);
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

    schedule(cocos2d::SEL_SCHEDULE(&GAFAnimatedObject::processAnimations));
    _animationsSelectorScheduled = true;
}

void GAFAnimatedObject::stop()
{
    GAFAnimation::stop();

    unschedule(cocos2d::SEL_SCHEDULE(&GAFAnimatedObject::processAnimations));
    _animationsSelectorScheduled = false;
}

int GAFAnimatedObject::numberOfGlobalFramesForOneAnimationFrame()
{
    float globalFPS = roundf(1.0f / (float)cocos2d::Director::getInstance()->getAnimationInterval());

    if (globalFPS > (float)m_fps - FLT_EPSILON)
    {
        return (int)roundf(globalFPS / (float)m_fps);
    }
    else
    {
        return 1;
    }
}

cocos2d::Sprite* GAFAnimatedObject::renderCurrentFrameToTexture(bool usePOTTextures)
{
    cocos2d::Rect frameRect = realBoundingBoxForCurrentFrame();

    if (frameRect.size.width == 0.f || frameRect.size.height == 0.f)
    {
        return NULL;
    }

    cocos2d::Vect originalPos = getPosition();

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

    cocos2d::RenderTexture* rt = cocos2d::RenderTexture::create(width, height, cocos2d::Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);

    setPosition(cocos2d::Vect(originalPos.x - frameRect.origin.x, originalPos.y - frameRect.origin.y));

#if DEBUG_RTT_DRAWING
    rt->beginWithClear(0, 0, 0, 0);
#else
    rt->begin();
#endif

    visit();

    rt->end();

    setPosition(originalPos);

    cocos2d::Sprite* res = cocos2d::Sprite::createWithTexture(rt->getSprite()->getTexture());
    res->setAnchorPoint(cocos2d::Vect(0, 0));
    res->setPosition(frameRect.origin);
    return(res);
}

void GAFAnimatedObject::realizeFrame(cocos2d::Node* out, size_t frameIndex)
{
    const AnimationFrames_t& animationFrames = m_asset->getAnimationFrames();

    if (frameIndex >= 0 && animationFrames.size() > frameIndex)
    {
        GAFAnimationFrame *currentFrame = animationFrames[frameIndex];

        for (SubObjectsList_t::iterator i = m_visibleObjects.begin(), e = m_visibleObjects.end(); i != e; ++i)
        {
            (*i)->setVisible(false);
        }

        m_visibleObjects.clear();

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
                        cocos2d::Vect prevAP = subObject->getAnchorPoint();
                        cocos2d::Size  prevCS = subObject->getContentSize();
#if ENABLE_RUNTIME_FILTERS
                        // Validate sprite type (w/ or w/o filter)
                        const Filters_t& filters = state->getFilters();
                        GAFFilterData* filter = NULL;

                        if (!filters.empty())
                        {
                            filter = filters[0];
                            filter->apply(subObject);
                        }

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
                            cocos2d::AffineTransform stateTransform = state->affineTransform;
                            float csf = m_asset->usedAtlasContentScaleFactor();
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

                            if (mask->getLocalZOrder() != state->zIndex)
                            {
                                mask->setLocalZOrder(state->zIndex);
                            }
                        }
                    }
                }
            }
        }

        if (m_controlDelegate)
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
                    if (subobjectCaptured && m_controlDelegate)
                    {
                        m_controlDelegate->onFrameDisplayed(this, subObject);
                    }
                }
                else
                {
                    // Masks cannot be captured right now
                }
            }
        }
    }
}

void GAFAnimatedObject::processAnimation()
{
    realizeFrame(this, _currentFrameIndex);
}

void GAFAnimatedObject::setFramePlayedDelegate(GAFFramePlayedDelegate * delegate)
{
    m_framePlayedDelegate = delegate;
}

void GAFAnimatedObject::setControlDelegate(GAFAnimatedObjectControlDelegate * delegate)
{
    m_controlDelegate = delegate;
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

cocos2d::Rect GAFAnimatedObject::realBoundingBoxForCurrentFrame()
{
    cocos2d::Rect result = cocos2d::Rect::ZERO;

    for (SubObjects_t::iterator i = m_subObjects.begin(), e = m_subObjects.end(); i != e; ++i)
    {
        GAFSprite* anim = i->second;
        if (anim->isVisible())
        {
            cocos2d::Rect bb = anim->getBoundingBox();
            if (i == m_subObjects.begin())
                result = bb;
            else
                result = GAFCCRectUnion(result, bb);
        }
    }

    return cocos2d::RectApplyTransform(result, getNodeToParentTransform());
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


void GAFAnimatedObject::_updateStencilLayer(int newLayer)
{
    m_stencilLayer = newLayer;

    for (SubObjects_t::iterator it = m_masks.begin(), e = m_masks.end(); it != e; ++it)
    {
        GAFStencilMaskSprite* mask = static_cast<GAFStencilMaskSprite*>(it->second);
        mask->updateStencilLayer(newLayer);
    }
}

int GAFAnimatedObject::getFps() const 
{
    return m_fps;
}

void GAFAnimatedObject::setFps(int value)
{
    CCASSERT(value, "Error! Fps is set to zero.");
    m_fps = value;
}
