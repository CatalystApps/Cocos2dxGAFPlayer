#pragma once

#include "GAFSprite.h"

static const char* kGAFStencilMaskAlphaFilterProgramCacheKey = "kGAFStencilMaskAlphaFilterProgramCacheKey";

namespace cocos2d
{
    class __Array;
}

class GAFStencilMaskSprite : public GAFSprite
{
private:
    struct StencilState
    {
        GLboolean   currentStencilEnabled;
        GLuint      currentStencilWriteMask;
        GLenum      currentStencilFunc;
        GLint       currentStencilRef;
        GLuint      currentStencilValueMask;
        GLenum      currentStencilFail;
        GLenum      currentStencilPassDepthFail;
        GLenum      currentStencilPassDepthPass;
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
    
    void addMaskedObject(cocos2d::Node * anObject);
    void removeMaskedObject(cocos2d::Node * anObject);
    void updateStencilLayer(int newLayer);

protected:
    void beginStencil(cocos2d::Mat4& transform);
    void endStencil();

private:
    void                    _sortAllMaskedObjects();
    void                    _disableStencil();
    int                     m_stencilLayer;
    bool                    m_isReorderMaskedObjectsDirty;

    cocos2d::GroupCommand   m_group;
    cocos2d::CustomCommand  m_customCommand2;
    cocos2d::__Array*       m_maskedObjects;
};
