#pragma once

#include "GAFSprite.h"

NS_GAF_BEGIN

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
    GAFColorColorMatrixFilterData*  m_colorMatrixFilterData;
    GAFGlowFilterData*              m_glowFilterData;
    GAFBlurFilterData*              m_blurFilterData;
    cocos2d::Texture2D *            m_initialTexture;
    cocos2d::Rect                   m_initialTextureRect;
    cocos2d::GLProgramState*        m_programBase;
    cocos2d::GLProgramState*        m_programNoCtx;
    mutable bool                    m_ctxDirty;

public:
    GAFSpriteWithAlpha();
    ~GAFSpriteWithAlpha();
    virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated);

    void setColorTransform(const GLfloat * mults, const GLfloat * offsets);
    void setColorTransform(const GLfloat * colorTransform);

    void setColorMarixFilterData(GAFColorColorMatrixFilterData* data);
    void setGlowFilterData(GAFGlowFilterData* data);
    void setBlurFilterData(GAFBlurFilterData* data);

    cocos2d::Texture2D*    getInitialTexture() const;
    const cocos2d::Rect&   getInitialTextureRect() const;

    bool            hasCtx();
    void            updateCtx();

protected:
    void updateTextureWithEffects();
    virtual uint32_t setUniforms() override;

};

NS_GAF_END
