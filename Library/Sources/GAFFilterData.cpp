#include "GAFPrecompiled.h"
#include "GAFFilterData.h"

#include "GAFMovieClip.h"
#include "GAFFilterManager.h"

NS_GAF_BEGIN

GAFBlurFilterData::GAFBlurFilterData():
GAFFilterData(GAFFilterType::GFT_Blur)
{

}

void GAFBlurFilterData::apply(GAFMovieClip* subObject)
{
    subObject->setBlurFilterData(this);
}

GAFColorColorMatrixFilterData::GAFColorColorMatrixFilterData():
GAFFilterData(GAFFilterType::GFT_ColorMatrix)
{

}

void GAFColorColorMatrixFilterData::apply(GAFMovieClip* subObject)
{
    subObject->setColorMarixFilterData(this);
}

GAFGlowFilterData::GAFGlowFilterData():
GAFFilterData(GAFFilterType::GFT_Glow)
{

}

void GAFGlowFilterData::apply(GAFMovieClip* subObject)
{
    subObject->setGlowFilterData(this);
}

GAFDropShadowFilterData::GAFDropShadowFilterData():
GAFFilterData(GAFFilterType::GFT_DropShadow)
{

}

const int kShadowObjectTag = 0xFAD0;

void GAFDropShadowFilterData::apply(GAFMovieClip* subObject)
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

void GAFDropShadowFilterData::reset(GAFMovieClip* subObject)
{
    cocos2d::Node* prevShadowObject = subObject->getChildByTag(kShadowObjectTag);

    if (prevShadowObject)
    {
        subObject->removeChild(prevShadowObject, true);
    }
}

NS_GAF_END