#pragma once

#ifndef __GAF_SPRITE__
#define __GAF_SPRITE__

typedef struct _gafBlendFuncSeparate
{
    // source blend function
    GLenum src;
    // destination blend function
    GLenum dst;
    // source alpha channel blend function
    GLenum srcAlpha;
    // destination alpha channel blend function
    GLenum dstAlpha;

} gafBlendFuncSeparate;

/// @class GAFSprite 
///	This is utility class used by GAF playback. It does not perform rendering or use OpenGL.
/// Instead, it references backend object via GAFSprite::getExternalPointer.

class GAFSprite : public cocos2d::Sprite
{
protected:
    cocos2d::AffineTransform m_externalTransform;
    cocos2d::AffineTransform m_childTransform;
    void invalidateTransformCache();
private:
    gafBlendFuncSeparate m_blendFuncSeparate;
    bool m_useSeparateBlendFunc;
    bool m_isLocator;
    GLint m_blendEquation;
    float m_atlasScale;

    cocos2d::QuadCommand      m_quadCommand;
    cocos2d::V3F_C4B_T2F_Quad m_quad;
public:
    GAFSprite();
    void setExternaTransform(const cocos2d::AffineTransform& transform);
    void setChildTransform(const cocos2d::AffineTransform& transform);

    unsigned int objectIdRef;

    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated);
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated);
    virtual void setUniformsForFragmentShader();

    inline const cocos2d::AffineTransform &childTransform() const
    {
        return m_childTransform;
    }
    const cocos2d::AffineTransform& getExternalTransform() const
    {
        return m_externalTransform;
    }
    inline void setLocator(bool locator)
    {
        m_isLocator = locator;
    }
    void setAtlasScale(float scale);

    virtual const cocos2d::Mat4& getNodeToParentTransform() const override;
    cocos2d::AffineTransform getNodeToParentAffineTransform() const;

    void setTexture(cocos2d::Texture2D *texture);
    bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated);

    virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
};


#endif // __GAF_SPRITE__
