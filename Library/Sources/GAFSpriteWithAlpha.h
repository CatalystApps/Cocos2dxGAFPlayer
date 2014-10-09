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
    GLfloat _colorTransform[8]; // 0-3 mults, 4-7 offsets

    GLfloat m_colorMatrixIdentity1[16];
    GLfloat m_colorMatrixIdentity2[4];

    GAFColorColorMatrixFilterData* m_colorMatrixFilterData;
    GAFGlowFilterData*             m_glowFilterData;
    GAFBlurFilterData*             m_blurFilterData;

    cocos2d::CCTexture2D * m_initialTexture;
    cocos2d::CCRect        m_initialTextureRect;

public:
    GAFSpriteWithAlpha();
    ~GAFSpriteWithAlpha();
    virtual void setUniformsForFragmentShader();
    virtual bool initWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect, bool rotated);

    cocos2d::CCGLProgram * programForShader();

    void setColorTransform(const GLfloat * mults, const GLfloat * offsets);
    const GLfloat * getColorTransform() const;

    void setColorTransform(const GLfloat * colorTransform);

    void setColorMarixFilterData(GAFColorColorMatrixFilterData* data);
    void setGlowFilterData(GAFGlowFilterData* data);
    void setBlurFilterData(GAFBlurFilterData* data);

    cocos2d::CCTexture2D*    getInitialTexture() const;
    const cocos2d::CCRect&   getInitialTextureRect() const;
    
    bool            isCTXIdentity() const;

    static void     reset();

protected:
    void updateTextureWithEffects();

};

#endif // __GAF_SPRITE_WITH_ALPHA__
