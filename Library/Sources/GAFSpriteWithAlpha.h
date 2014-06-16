#pragma once

#ifndef __GAF_SPRITE_WITH_ALPHA__
#define __GAF_SPRITE_WITH_ALPHA__

#include "GAFSprite.h"

class GAFColorColorMatrixFilterData;
class GAFGlowFilterData;
class GAFBlurFilterData;

class GAFSpriteWithAlpha : public GAFSprite
{
private:
    void _setBlendingFunc();

private:
    cocos2d::Vec4                   m_colorTransformMult;
    cocos2d::Vec4                   m_colorTransformOffsets;
    cocos2d::Mat4                   m_colorMatrixIdentity1;
    cocos2d::Vec4                   m_colorMatrixIdentity2;
    GAFColorColorMatrixFilterData*  m_colorMatrixFilterData;
    GAFGlowFilterData*              m_glowFilterData;
    GAFBlurFilterData*              m_blurFilterData;
    cocos2d::Texture2D *            m_initialTexture;
    cocos2d::Rect                   m_initialTextureRect;

public:
    GAFSpriteWithAlpha();
    ~GAFSpriteWithAlpha();
    virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated);

    cocos2d::GLProgram * programForShader();

    void setColorTransform(const GLfloat * mults, const GLfloat * offsets);
    void setColorTransform(const GLfloat * colorTransform);

    void setColorMarixFilterData(GAFColorColorMatrixFilterData* data);
    void setGlowFilterData(GAFGlowFilterData* data);
    void setBlurFilterData(GAFBlurFilterData* data);

    cocos2d::Texture2D*    getInitialTexture() const;
    const cocos2d::Rect&   getInitialTextureRect() const;

    bool            isCTXIdentity() const;

protected:
    void updateTextureWithEffects();
    virtual uint32_t setUniforms() override;

};

#endif // __GAF_SPRITE_WITH_ALPHA__
