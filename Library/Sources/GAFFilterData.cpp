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
    cocos2d::Texture2D* texture = subObject->getInitialTexture();
    const cocos2d::Rect& texRect = subObject->getInitialTextureRect();
    cocos2d::RenderTexture* shadowObject = GAFTextureEffectsConverter::sharedConverter()->dropShadowTextureFromTexture(texture, texRect, this);

    reset(subObject);

    shadowObject->setTag(kShadowObjectTag);

    const float anglerad = ((float)M_PI / 180.f) * angle;

    cocos2d::Vect offset = cocos2d::Vect(cos(anglerad) * distance, -sin(anglerad) * distance);

    cocos2d::Size shadowTextureSize = shadowObject->getSprite()->getContentSize();
    if (texRect.size.height < shadowTextureSize.height)
    {
        offset.y -= shadowTextureSize.height - texRect.size.height;
    }

    shadowObject->setPosition(offset);
    subObject->addChild(shadowObject, -1);
}

void GAFDropShadowFilterData::reset(GAFSpriteWithAlpha* subObject)
{
    cocos2d::Node* prevShadowObject = subObject->getChildByTag(kShadowObjectTag);

    if (prevShadowObject)
    {
        subObject->removeChild(prevShadowObject, true);
    }
}
