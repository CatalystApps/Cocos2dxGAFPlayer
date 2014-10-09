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

class GAFSprite : public cocos2d::CCSprite
{
public:
    GAFSprite();
    void setExternaTransform(const cocos2d::CCAffineTransform& transform);
    void setChildTransform(const cocos2d::CCAffineTransform& transform);

    unsigned int objectIdRef;

    virtual void draw();
    virtual void setUniformsForFragmentShader();

    inline const cocos2d::CCAffineTransform &childTransform() const
    {
        return _childTransform;
    }
    const cocos2d::CCAffineTransform& getExternalTransform() const
    {
        return _externalTransform;
    }
    inline void setLocator(bool locator)
    {
        _isLocator = locator;
    }
    void setAtlasScale(float scale);
    cocos2d::CCAffineTransform nodeToParentTransform(void);
    void setTexture(cocos2d::CCTexture2D *texture);
    bool initWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect, bool rotated);
protected:
    cocos2d::CCAffineTransform _externalTransform;
    cocos2d::CCAffineTransform _childTransform;
    void invalidateTransformCache();
private:
    gafBlendFuncSeparate _blendFuncSeparate;
    bool _useSeparateBlendFunc;
    bool _isLocator;
    GLint _blendEquation;
    float _atlasScale;
};


#endif // __GAF_SPRITE__
