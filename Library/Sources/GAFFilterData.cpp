#include "GAFPrecompiled.h"
#include "GAFFilterData.h"

#include "GAFSpriteWithAlpha.h"
#include "GAFTextureEffectsConverter.h"

GAFBlurFilterData::GAFBlurFilterData():
GAFFilterData(GFT_Blur)
{

}

void GAFBlurFilterData::apply(GAFSpriteWithAlpha* subObject)
{
    subObject->setBlurFilterData(this);
}

GAFColorColorMatrixFilterData::GAFColorColorMatrixFilterData():
GAFFilterData(GFT_ColorMatrix)
{

}

void GAFColorColorMatrixFilterData::apply(GAFSpriteWithAlpha* subObject)
{
    subObject->setColorMarixFilterData(this);
}

GAFGlowFilterData::GAFGlowFilterData():
GAFFilterData(GFT_Glow)
{

}

void GAFGlowFilterData::apply(GAFSpriteWithAlpha* subObject)
{
    subObject->setGlowFilterData(this);
}

GAFDropShadowFilterData::GAFDropShadowFilterData():
GAFFilterData(GFT_DropShadow)
{

}

const int kShadowObjectTag = 0xFAD0;

void GAFDropShadowFilterData::apply(GAFSpriteWithAlpha* subObject)
{
    CCTexture2D* texture = subObject->getInitialTexture();
    const CCRect& texRect = subObject->getInitialTextureRect();
    CCRenderTexture* shadowObject = GAFTextureEffectsConverter::sharedConverter()->dropShadowTextureFromTexture(texture, texRect, this);

    reset(subObject);

    shadowObject->setTag(kShadowObjectTag);

    const float anglerad = ((float)M_PI / 180.f) * angle;

    CCPoint offset = ccp(cos(anglerad) * distance, -sin(anglerad) * distance);

    CCSize shadowTextureSize = shadowObject->getSprite()->getContentSize();
    if (texRect.size.height < shadowTextureSize.height)
    {
        offset.y -= shadowTextureSize.height - texRect.size.height;
    }

    shadowObject->setPosition(offset);
    subObject->addChild(shadowObject, -1);
}

void GAFDropShadowFilterData::reset(GAFSpriteWithAlpha* subObject)
{
    CCNode* prevShadowObject = subObject->getChildByTag(kShadowObjectTag);

    if (prevShadowObject)
    {
        subObject->removeChild(prevShadowObject, true);
    }
}
