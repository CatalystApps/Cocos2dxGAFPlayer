#include "GAFTextureAtlasElement.h"

#include "cocoa/CCInteger.h"
#include "CCJSONData.h"
#include "cocoa/CCDictionary.h"

static const char * kNameKey    = "name";
static const char * kXKey       = "x";
static const char * kYKey       = "y";
static const char * kScaleKey   = "scale";
static const char * kHeightKey  = "height";
static const char * kWidthKey   = "width";
static const char * kPivotXKey  = "pivotX";
static const char * kPivotYKey  = "pivotY";
static const char * kAtlasIDKey = "atlasID";

GAFTextureAtlasElement::GAFTextureAtlasElement()
{
	
}

GAFTextureAtlasElement * GAFTextureAtlasElement::create(CCDictionary * aDictionary)
{
	GAFTextureAtlasElement * ret = new GAFTextureAtlasElement();
	if (ret && ret->initWithDictionary(aDictionary))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return NULL;
}

bool GAFTextureAtlasElement::initWithDictionary(CCDictionary * aDictionary)
{
	CCString * jName = (CCString*)aDictionary->objectForKey(kNameKey);
	if (jName)
	{
		name = std::string(jName->getCString());
	}
	
	CCNumber  * s   = (CCNumber *)  aDictionary->objectForKey(kScaleKey);
	if (!s)
	{
		scale = 1.0f;
	}
	else
	{
		scale = (float)s->getDoubleValue();
	}

	CCInteger * nX      = (CCInteger*)aDictionary->objectForKey(kXKey);
	CCInteger * nY      = (CCInteger*)aDictionary->objectForKey(kYKey);
	CCInteger * nWidth  = (CCInteger*)aDictionary->objectForKey(kWidthKey);
	CCInteger * nHeight = (CCInteger*)aDictionary->objectForKey(kHeightKey);

	if (nX && nY && nHeight && nWidth)
	{
		bounds = CCRectMake(nX->getValue(), nY->getValue(), nWidth->getValue(), nHeight->getValue());
	}
	else
	{
		CCLOGERROR("Error while creating GAFTextureAtlasElement from NSDictionary. One or more size attributes missing.");
		return false;
	}
	
	CCInteger * nPivotX      = (CCInteger*)aDictionary->objectForKey(kPivotXKey);
	CCInteger * nPivotY      = (CCInteger*)aDictionary->objectForKey(kPivotYKey);
	
	if (nPivotX && nPivotY)
	{
		pivotPoint.x = nPivotX->getValue();
		pivotPoint.y = nPivotY->getValue();
	}
	else
	{
		CCLOGERROR("Error while creating GAFTextureAtlasElement from NSDictionary. One or more pivot attributes missing.");
		return false;
	}
	CCInteger * atlasId = (CCInteger*)aDictionary->objectForKey(kAtlasIDKey);
	if (atlasId)
	{
		atlasIdx = atlasId->getValue() - 1;
	}
	else
	{
		atlasIdx = 0;
		CCLOGWARN("AtlasID is missing, assuming first atlas is used.");
	}
	return true;
}
