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
    GLfloat m_colorTransform[8]; // 0-3 mults, 4-7 offsets

    GLfloat m_colorMatrixIdentity1[16];
    GLfloat m_colorMatrixIdentity2[4];

    GAFColorColorMatrixFilterData* m_colorMatrixFilterData;
    GAFGlowFilterData*             m_glowFilterData;
    GAFBlurFilterData*             m_blurFilterData;

    cocos2d::Texture2D * m_initialTexture;
    cocos2d::Rect        m_initialTextureRect;
    cocos2d::RenderTexture* m_filter;

    cocos2d::RenderTexture* _test();

public:
    GAFSpriteWithAlpha();
    ~GAFSpriteWithAlpha();
    virtual void setUniformsForFragmentShader();
    virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated);

    cocos2d::GLProgram * programForShader();

    void setColorTransform(const GLfloat * mults, const GLfloat * offsets);
    const GLfloat * getColorTransform() const;

    void setColorTransform(const GLfloat * colorTransform);

    void setColorMarixFilterData(GAFColorColorMatrixFilterData* data);
    void setGlowFilterData(GAFGlowFilterData* data);
    void setBlurFilterData(GAFBlurFilterData* data);

    cocos2d::Texture2D*    getInitialTexture() const;
    const cocos2d::Rect&   getInitialTextureRect() const;

    bool            isCTXIdentity() const;

protected:
    void updateTextureWithEffects();

};

#endif // __GAF_SPRITE_WITH_ALPHA__
