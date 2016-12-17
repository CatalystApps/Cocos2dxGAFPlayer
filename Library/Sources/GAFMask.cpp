#include "GAFPrecompiled.h"
#include "GAFMask.h"
#include "GAFShaderManager.h"
#include "base/ccMacros.h"

#define USE_LAYERED_STENCIL 0

NS_GAF_BEGIN

GAFMask::GAFMask()
{
    m_charType = GAFCharacterType::Texture;
    m_objectType = GAFObjectType::Mask;
}

GAFMask::~GAFMask()
{
}

bool GAFMask::initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated)
{
    if (!Sprite::initWithTexture(pTexture, cocos2d::Rect(0, 0, rect.size.width, rect.size.height), rotated))
    {
        return false;
    }
    return true;
}

void GAFMask::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    return Sprite::draw(renderer, transform, flags);
}

void GAFMask::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    // Clipping node works well only with cocos sprite for now.
    return Sprite::visit(renderer, transform, flags);
}

NS_GAF_END
