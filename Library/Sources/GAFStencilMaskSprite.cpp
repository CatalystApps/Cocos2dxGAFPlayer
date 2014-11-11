#include "GAFPrecompiled.h"
#include "GAFStencilMaskSprite.h"
#include "GAFShaderManager.h"

NS_GAF_BEGIN

GAFStencilMaskSprite::GAFStencilMaskSprite()
{
    m_type = GAFSprite::ESpriteType::Mask;
}

bool GAFStencilMaskSprite::initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated)
{
    if (!Sprite::initWithTexture(pTexture, cocos2d::Rect(0, 0, rect.size.width, rect.size.height), rotated))
    {
        return false;
    }
    return true;
}

GAFStencilMaskSprite::~GAFStencilMaskSprite()
{
}

#if COCOS2D_VERSION < 0x00030200
void GAFStencilMaskSprite::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool flags)
#else
void GAFStencilMaskSprite::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
#endif
{
    return Sprite::visit(renderer, transform, flags);
}

#if COCOS2D_VERSION < 0x00030200
void GAFStencilMaskSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool flags)
#else
void GAFStencilMaskSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
#endif
{
    return Sprite::draw(renderer, transform, flags);
}

NS_GAF_END
