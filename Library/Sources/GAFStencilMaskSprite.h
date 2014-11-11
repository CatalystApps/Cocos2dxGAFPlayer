#pragma once

#include "GAFSprite.h"

NS_GAF_BEGIN

class GAFStencilMaskSprite : public GAFSprite
{
public:
    GAFStencilMaskSprite();
    ~GAFStencilMaskSprite();
    virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated);

#if COCOS2D_VERSION < 0x00030200
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated) override;
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated) override;
#else
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
#endif
};

NS_GAF_END
