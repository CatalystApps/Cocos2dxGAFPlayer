#pragma once

#ifndef __GAF_STENCIL_MASK_SPRITE__
#define __GAF_STENCIL_MASK_SPRITE__

#include "GAFSprite.h"

namespace cocos2d
{
    class CCArray;
}

#define  kGAFStencilMaskAlphaFilterProgramCacheKey "kGAFScrollLayerAlphaFilterProgramCacheKey"

using namespace cocos2d;

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
    virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated);
    static CCGLProgram * programShaderForMask();
    virtual void draw();
    virtual void visit();

    void invalidateMaskedObjectsOrder();

    static void updateMaskContainerOf(CCNode * node);

    void addMaskedObject(CCNode * anObject);
    void removeMaskedObject(CCNode * anObject);

    void updateStencilLayer(int newLayer);

protected:
    void sortAllMaskedObjects();
private:
    int       m_stencilLayer;
    CCArray * _maskedObjects;
    bool     _isReorderMaskedObjectsDirty;
    void     _setupStencilForMask();
    void     _setupStencilForContent();
    void     _disableStencil();
};

#endif // __GAF_STENCIL_MASK_SPRITE__
