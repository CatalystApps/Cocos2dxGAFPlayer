#include "GAFAsset.h"
#include "GAFAnimatedObject.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFAnimationFrame.h"
#include "GAFSubobjectState.h"
#include "GAFSpriteWithAlpha.h"
#include "GAFStencilMaskSprite.h"
#include "GAFFilterData.h"

#include "CCDirector.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCInteger.h"
#include "textures/CCTexture2D.h"
#include "sprite_nodes/CCSpriteFrame.h"
#include <limits>

#include "support/CCPointExtension.h"


#ifdef max
	#undef max
#endif


static const char * kGAFBlurFilterName = "Fblur";
static float const kAnimationFPS = 30.0;   // we keep this number 'almost' synchronized with web. The reason it's
// not 31 fps is that we will have some animation artifacts when running
// on 30/60 fps device.

static CCAffineTransform GAF_CGAffineTransformCocosFormatFromFlashFormat(CCAffineTransform aTransform)
{
    CCAffineTransform transform = aTransform;
    transform.b = - transform.b;
    transform.c = - transform.c;
    transform.ty = - transform.ty;
    return transform;
}

GAFAnimatedObject::GAFAnimatedObject()
:
_asset(NULL),
_subObjects(NULL),
_masks(NULL),
_capturedObjects(NULL),
_extraFramesCounter(0),
_framePlayedDelegate(NULL),
_controlDelegate(NULL)
{
}

GAFAnimatedObject::~GAFAnimatedObject()
{
	CC_SAFE_RELEASE(_asset);
	CC_SAFE_RELEASE(_subObjects);
	CC_SAFE_RELEASE(_masks);
	CC_SAFE_RELEASE(_capturedObjects);
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


GAFAnimatedObject * GAFAnimatedObject::create(const char * jsonPath)
{
	GAFAsset * asset = GAFAsset::create(jsonPath);
	return asset->createObject();
}


GAFAnimatedObject * GAFAnimatedObject::createAndRun(const char * jsonPath, bool looped)
{
	GAFAsset * asset = GAFAsset::create(jsonPath);
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
	CC_SAFE_RELEASE(_subObjects);
	_subObjects = CCDictionary::create();
	CC_SAFE_RETAIN(_subObjects);
	
	CC_SAFE_RELEASE(_masks);
	_masks = CCDictionary::create();
	CC_SAFE_RETAIN(_masks);
	
	CC_SAFE_RELEASE(_capturedObjects);
	_capturedObjects = CCDictionary::create();
	CC_SAFE_RETAIN(_capturedObjects);
	
	_FPSType = kGAFAnimationFPSType_30;
	_extraFramesCounter = 0;
	_animationsSelectorScheduled = false;	
	return true;
}

static bool util_ccarray_contains_string(CCArray * array, const std::string& str)
{
	if (!array)
	{
		return false;
	}
	for (int i = 0; i < array->count(); ++i)
	{
		CCString * obj = (CCString *)array->objectAtIndex(i);
		if (obj->getCString() == str)
		{
			return true;
		}
	}
	
	return false;
}

GAFSprite * GAFAnimatedObject::subobjectByName(const char * name)
{
	std::string rawName = objectIdByObjectName(name);
	if (!rawName.length())
	{
		return 0;
	}
	return subobjectByRawName(rawName.c_str());
}

GAFSprite * GAFAnimatedObject::subobjectByRawName(const char * name)
{
	if (!_subObjects)
	{
		return 0;
	}
	return (GAFSprite *)_subObjects->objectForKey(name);
}

GAFSprite * GAFAnimatedObject::subObjectForInnerObjectId(const char * anInnerObjectId)
{
	CCDictElement* pElement = 0;
	CCDICT_FOREACH(_subObjects, pElement)
    {
		GAFSprite * obj = (GAFSprite*)pElement->getObject();
		if (obj->objectId == anInnerObjectId)
		{
			return obj;
		}
    }
	return 0;
}

std::string GAFAnimatedObject::objectIdByObjectName(const char * name)
{
	if (!name)
	{
		return 0;
	}
	CCDictionary * namedParts = _asset->namedParts();
	if (!namedParts)
	{
		return 0;
	}
	CCDictElement* pElement = 0;
    CCDICT_FOREACH(namedParts, pElement)
    {
		CCString * str = (CCString*)pElement->getObject();
		if (0 == strcmp(str->getCString(), name))
		{
			return pElement->getStrKey();
		}
    }
	return std::string();
}

void GAFAnimatedObject::addSubObjectsUsingAnimationObjectsDictionary(CCDictionary * anAnimationObjects,
CCDictionary * anAnimationMasks, CCArray * anAnimationFrames)
{
	CCDictElement* pElement = NULL;
    CCDICT_FOREACH(anAnimationObjects, pElement)
    {
		
        CCString * atlasElementId = (CCString *)pElement->getObject();
        CCSpriteFrame * spriteFrame = NULL;
		GAFTextureAtlas * atlas = _asset->textureAtlas();
		
        GAFTextureAtlasElement *element = (GAFTextureAtlasElement *)atlas->elements()->objectForKey(atlasElementId->getCString());
		
		if (element)
		{
			if (atlas->textures()->count() >= element->atlasIdx + 1)
			{
				CCTexture2D * texture = (CCTexture2D *)atlas->textures()->objectAtIndex(element->atlasIdx);
				spriteFrame = CCSpriteFrame::createWithTexture(texture, element->bounds);
			}
			else
			{
				CCLOGERROR("Cannot add sub object with Id: %s, atlas with idx: %d not found.", atlasElementId->getCString(), element->atlasIdx);
			}
		}
		        
        if (spriteFrame)
        {
            GAFSpriteWithAlpha *sprite = new GAFSpriteWithAlpha();
			sprite->initWithSpriteFrame(spriteFrame);
			
            sprite->objectId = atlasElementId->getCString();
            
            sprite->setVisible(false);
			CCPoint pt = CCPointMake(0 - (0 - (element->pivotPoint.x / sprite->getContentSize().width)),
									 0 + (1 - (element->pivotPoint.y / sprite->getContentSize().height)));
            sprite->setAnchorPoint(pt);
			
			if (element->scale != 1.0f)
			{
				sprite->setAtlasScale(1.0f / element->scale);
			}
			// visual studio compile fix
			ccBlendFunc blend = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
			sprite->setBlendFunc(blend);   
			_subObjects->setObject(sprite, pElement->getStrKey());
			sprite->release();
        }
        else
        {
            CCLOGERROR("Cannot add subnode with AtlasElementName: %s, not found in atlas(es). Ignoring.", atlasElementId->getCString());
        }
    }
    

	CCDICT_FOREACH(anAnimationMasks, pElement)
	{
		CCString * atlasElementId = (CCString *)pElement->getObject();
		
		CCSpriteFrame *spriteFrame = NULL;
		GAFTextureAtlas * atlas = _asset->textureAtlas();
		GAFTextureAtlasElement *element = (GAFTextureAtlasElement *)atlas->elements()->objectForKey(atlasElementId->getCString());
		
		if (element)
		{
			spriteFrame = CCSpriteFrame::createWithTexture(atlas->texture(), element->bounds);
			
			if (spriteFrame)
			{
				GAFStencilMaskSprite *mask = new GAFStencilMaskSprite();
				mask->initWithSpriteFrame(spriteFrame);
				mask->objectId = pElement->getStrKey();				
				CCPoint pt = CCPointMake(0 - (0 - (element->pivotPoint.x / mask->getContentSize().width)),
										 0 + (1 - (element->pivotPoint.y / mask->getContentSize().height)));
				
				mask->setAnchorPoint(pt);				
				if (element->scale != 1.0f)
				{
					mask->setAtlasScale(1.0f / element->scale);
				}
				_masks->setObject(mask, pElement->getStrKey());
				addChild(mask);
				mask->release();
			}
		}
	}
}

bool GAFAnimatedObject::captureControlOverSubobjectNamed(const char * aName, GAFAnimatedObjectControlFlags aControlFlags)
{
	std::string objectId = objectIdByObjectName(aName);
	if (0 == objectId.length())
	{
		return false;
	}
	if (_capturedObjects->objectForKey(objectId))
	{
		return false;
	}
	_capturedObjects->setObject(CCInteger::create(aControlFlags), objectId);
	return true;
}

void GAFAnimatedObject::releaseControlOverSubobjectNamed(const char * aName)
{
	std::string objectId = objectIdByObjectName(aName);
	if (objectId.length())
	{
		_capturedObjects->removeObjectForKey(objectId);
	}	
}

void GAFAnimatedObject::removeAllSubObjects()
{
	CCDictElement* pElement = NULL;
    CCDICT_FOREACH(_subObjects, pElement)
    {
        GAFSprite* sprite = (GAFSprite*)pElement->getObject();
		sprite->setVisible(false);
		sprite->removeFromParentAndCleanup(true);
    }
	_subObjects->removeAllObjects();
}

void GAFAnimatedObject::setSubobjectsVisible(bool visible)
{
	CCDictElement* pElement = NULL;
    CCDICT_FOREACH(_subObjects, pElement)
    {
        CCSprite* sprite = (CCSprite*)pElement->getObject();
		sprite->setVisible(visible);
    }
}

void GAFAnimatedObject::processAnimations(float dt)
{
	if (++_extraFramesCounter >= numberOfGlobalFramesForOneAnimationFrame())
    {
        _extraFramesCounter = 0;
		if (!isDone() && isAnimationRunning())
		{
			step();
		}
		if (_framePlayedDelegate)
		{
			_framePlayedDelegate->onFramePlayed(this, currentFrameIndex());
		}
    }
}


CCPoint GAFAnimatedObject::pupilCoordinatesWithXSemiaxis(float anXSemiaxis, float anYSemiaxis, CCPoint aCenter, CCPoint anExternalPoint)
{
    CCPoint rePoint = CCPointZero;
    float const firstConst = (anYSemiaxis * anYSemiaxis);
    float const secondConst = firstConst/(anXSemiaxis * anXSemiaxis);
    float length = std::numeric_limits<float>::max();
    float tmpLenght = length;
    float xStart = - anXSemiaxis;
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
            y = - y;
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

GAFSprite * GAFAnimatedObject::subObjectForInnerObjectId(CCString * anInnerObjectId)
{
	CCDictElement* pElement = NULL;
    CCDICT_FOREACH(_subObjects, pElement)
    {
        GAFSprite* anim = (GAFSprite*)pElement->getObject();
		if (anim->objectId == anInnerObjectId->getCString())
		{
			return anim;
		}
    }
	return NULL;
	
}

CCDictionary * GAFAnimatedObject::subObjects()
{
	return _subObjects;
}

CCDictionary * GAFAnimatedObject::masks()
{
	return _masks;
}

void GAFAnimatedObject::start()
{

	GAFAnimation::start();
	removeAllSubObjects();
    addSubObjectsUsingAnimationObjectsDictionary(_asset->objects(), _asset->masks(), _asset->animationFrames());
	
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
	float globalFPS = roundf(1.0 / CCDirector::sharedDirector()->getAnimationInterval());
    
    if (globalFPS > (float)_FPSType - FLT_EPSILON)
    {
        return (int)roundf(globalFPS / (float)_FPSType);
    }
    else
    {
        return 1;
    }
}

void GAFAnimatedObject::processAnimation()
{
    GAFAnimationFrame *currentFrame = (GAFAnimationFrame*)_asset->animationFrames()->objectAtIndex(_currentFrameIndex);
	setSubobjectsVisible(false);
	
	CCArray * arr = currentFrame->objectStates();
	
	if (arr)
	{
		int n = arr->count();
		for (int i = 0; i < n; ++i)
		{			
			GAFSubobjectState *state = (GAFSubobjectState*)arr->objectAtIndex(i);
			{
				CCDictionary * subobjects = subObjects();
				GAFSpriteWithAlpha *subObject =  NULL;
				if (subobjects)
				{
					subObject = (GAFSpriteWithAlpha *)subobjects->objectForKey(state->objectId);
				}
				
				if (subObject)
				{					
					// Validate sprite type (w/ or w/o filter)
					GAFBlurFilterData *blurFilter = NULL;
					
					if (state->filters())
					{
						blurFilter = (GAFBlurFilterData *) state->filters()->objectForKey(kGAFBlurFilterName);
					}
					CCPoint prevAP = subObject->getAnchorPoint();
					CCSize  prevCS = subObject->getContentSize();
					
					if (blurFilter)
					{
						subObject->setBlurRadius(CCSizeMake(blurFilter->blurSize.width / 4, blurFilter->blurSize.height / 4));
					}
					else
					{
						subObject->setBlurRadius(CCSizeZero);
					}
				
					CCSize newCS = subObject->getContentSize();
					CCPoint newAP = CCPointMake( ((prevAP.x - 0.5) * prevCS.width) / newCS.width + 0.5,
												((prevAP.y - 0.5) * prevCS.height) / newCS.height + 0.5);
					subObject->setAnchorPoint(newAP);
					
					if (!state->maskObjectId.length())
					{
						if (!subObject->getParent())
						{
							addChild(subObject);
						}
					}
					else
					{
						if (subObject->getParent())
						{
							removeChild(subObject, false);
						}						
						GAFStencilMaskSprite * mask = NULL;
						if (_masks)
						{
							mask = (GAFStencilMaskSprite *)_masks->objectForKey(state->maskObjectId);
						}
						if (mask)
						{
							mask->addMaskedObject(subObject);
						}
					}
					
					bool subobjectCaptured = false;
					GAFAnimatedObjectControlFlags controlFlags = kGAFAnimatedObjectControl_None;
					CCInteger * flagsNum = (CCInteger *)_capturedObjects->objectForKey(state->objectId);
					if (flagsNum)
					{
						subobjectCaptured = true;
						controlFlags = (GAFAnimatedObjectControlFlags) flagsNum->getValue();
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
						subObject->setVisible(state->isVisisble());
						subObject->setColorTransform(state->colorMults(), state->colorOffsets());
					}
				}
				else
				{										
					GAFSprite * mask = NULL;
					if (_masks)
					{
						mask = (GAFSprite *)_masks->objectForKey(state->objectId);
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
	
	if (_controlDelegate && arr)
	{
		int n = arr->count();
		for (int i = 0; i < n; ++i)
		{
			
			GAFSubobjectState *state = (GAFSubobjectState*)arr->objectAtIndex(i);
			GAFSpriteWithAlpha *subObject = (GAFSpriteWithAlpha *) subObjects()->objectForKey(state->objectId);
			if (subObject)
			{
				bool subobjectCaptured = NULL != _capturedObjects->objectForKey(state->objectId);
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
    CCRect result;
    
    float x1 = MIN(src1.getMinX(), src2.getMinX());
    float y1 = MIN(src1.getMinY(), src2.getMinY());
    float x2 = MAX(src1.getMaxX(), src2.getMaxX());
    float y2 = MAX(src1.getMaxY(), src2.getMaxY());
    
    result.origin=ccp(x1,x2);
    result.size=CCSizeMake(x2-x1, y2-y1);
    return result;
}

CCRect GAFAnimatedObject::realBoundingBoxForCurrentFrame()
{

	CCRect result = CCRectZero;
	
	CCDictElement* pElement = NULL;
    CCDICT_FOREACH(_subObjects, pElement)
    {
        GAFSprite* anim = (GAFSprite*)pElement->getObject();
		if (anim->isVisible())
		{
			CCRect bb = anim->boundingBox();
			result = GAFCCRectUnion(result, bb);
		}		
    }	
	return CCRectApplyAffineTransform(result, nodeToParentTransform());
}


