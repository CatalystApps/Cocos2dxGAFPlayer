#pragma once

#ifndef __GAF_STENCIL_MASK_SPRITE__
#define __GAF_STENCIL_MASK_SPRITE__

#include "GAFSprite.h"

#define  kGAFStencilMaskAlphaFilterProgramCacheKey "kGAFScrollLayerAlphaFilterProgramCacheKey"

namespace cocos2d
{
    class __Array;
}

class GAFStencilMaskSprite : public GAFSprite
{
private:
    struct StencilState
    {
        GLboolean currentStencilEnabled;
        GLuint currentStencilWriteMask;
        GLenum currentStencilFunc;
        GLint currentStencilRef;
        GLuint currentStencilValueMask;
        GLenum currentStencilFail;
        GLenum currentStencilPassDepthFail;
        GLenum currentStencilPassDepthPass;
    } m_stencilState;

public:
    GAFStencilMaskSprite(int stencilLayer);
    ~GAFStencilMaskSprite();
    virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated);
    static cocos2d::GLProgram * programShaderForMask();

#if COCOS2D_VERSION < 0x00030200
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated) override;
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated) override;
#else
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
#endif

    void invalidateMaskedObjectsOrder();

    static void updateMaskContainerOf(cocos2d::Node * node);

    void addMaskedObject(cocos2d::Node * anObject);
    void removeMaskedObject(cocos2d::Node * anObject);
    void updateStencilLayer(int newLayer);
protected:
    void sortAllMaskedObjects();
private:
    int       m_stencilLayer;
    cocos2d::__Array * _maskedObjects;
    bool     _isReorderMaskedObjectsDirty;
    void     _disableStencil();
};

#endif // __GAF_STENCIL_MASK_SPRITE__
