#include "GAFPrecompiled.h"
#include "GAFFilterData.h"

#include "GAFSpriteWithAlpha.h"
#include "GAFFilterManager.h"

NS_GAF_BEGIN

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
    cocos2d::Texture2D* shadow = GAFFilterManager::getInstance()->applyFilter(cocos2d::Sprite::createWithTexture(texture, texRect), this);
    cocos2d::Sprite* shadowSprite = cocos2d::Sprite::createWithTexture(shadow);
    reset(subObject);

    shadowSprite->setTag(kShadowObjectTag);
    shadowSprite->setOpacity(static_cast<GLubyte>(cocos2d::clampf(strength, 0.0, 1.0) * 255));

    const float anglerad = ((float)M_PI / 180.f) * angle;
    cocos2d::Size shadowTextureSize = shadowSprite->getContentSize();
    cocos2d::Vect offset = cocos2d::Vect(cos(anglerad) * distance, -sin(anglerad) * distance);
    shadowSprite->setPosition(cocos2d::Vec2(texRect.size / 2) + offset);
    subObject->addChild(shadowSprite, -1);
}

void GAFDropShadowFilterData::reset(GAFSpriteWithAlpha* subObject)
{
    cocos2d::Node* prevShadowObject = subObject->getChildByTag(kShadowObjectTag);

    if (prevShadowObject)
    {
        subObject->removeChild(prevShadowObject, true);
    }
}

NS_GAF_END
