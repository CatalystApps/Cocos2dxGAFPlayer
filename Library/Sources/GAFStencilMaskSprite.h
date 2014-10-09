#pragma once

#ifndef __GAF_STENCIL_MASK_SPRITE__
#define __GAF_STENCIL_MASK_SPRITE__

#include "GAFSprite.h"

namespace cocos2d
{
    class CCArray;
}

#define  kGAFStencilMaskAlphaFilterProgramCacheKey "kGAFScrollLayerAlphaFilterProgramCacheKey"

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
    virtual bool initWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect, bool rotated);
    static cocos2d::CCGLProgram * programShaderForMask();
    virtual void draw();
    virtual void visit();

    void invalidateMaskedObjectsOrder();

    static void updateMaskContainerOf(cocos2d::CCNode * node);

    void addMaskedObject(cocos2d::CCNode * anObject);
    void removeMaskedObject(cocos2d::CCNode * anObject);
    void updateStencilLayer(int newLayer);

    static void reset();
protected:
    void sortAllMaskedObjects();
private:
    int                 m_stencilLayer;
    cocos2d::CCArray *  _maskedObjects;
    bool                _isReorderMaskedObjectsDirty;
    void                _setupStencilForMask();
    void                _setupStencilForContent();
    void                _disableStencil();
};

#endif // __GAF_STENCIL_MASK_SPRITE__
