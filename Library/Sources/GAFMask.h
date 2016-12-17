#pragma once

#include "GAFObject.h"

NS_GAF_BEGIN

class GAFMask : public GAFObject
{
public:
    GAFMask();
    ~GAFMask();
    virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated) override;

    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
};

NS_GAF_END
