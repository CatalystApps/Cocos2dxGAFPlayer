#pragma once

#include "GAFObject.h"

NS_GAF_BEGIN

class GAFColorColorMatrixFilterData;
class GAFGlowFilterData;
class GAFBlurFilterData;

class GAFMovieClip : public GAFObject
{
private:
    void _setBlendingFunc();

protected:
    cocos2d::Vec4                   m_colorTransformMult;
    cocos2d::Vec4                   m_colorTransformOffsets;
    cocos2d::Mat4                   m_colorMatrixIdentity1;
    cocos2d::Vec4                   m_colorMatrixIdentity2;
    GAFColorColorMatrixFilterData*  m_colorMatrixFilterData;
    GAFGlowFilterData*              m_glowFilterData;
    GAFBlurFilterData*              m_blurFilterData;
    cocos2d::Texture2D *            m_initialTexture;
    cocos2d::Rect                   m_initialTextureRect;
    cocos2d::GLProgramState*        m_programBase;
    cocos2d::GLProgramState*        m_programNoCtx;
    mutable bool                    m_ctxDirty;

    void updateTextureWithEffects();
    virtual uint32_t setUniforms() override;
public:

    GAFMovieClip();
    virtual ~GAFMovieClip();

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

#if COCOS2D_VERSION < 0x00030200
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated) override;
#else
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
#endif
};

NS_GAF_END