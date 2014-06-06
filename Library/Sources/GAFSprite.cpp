#include "GAFPrecompiled.h"
#include "GAFSprite.h"

#include "TransformUtils.h"

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__A__) ( (int)(__A__))
#endif
#include "GAFSpriteWithAlpha.h"

GAFSprite::GAFSprite()
:
m_useSeparateBlendFunc(false),
m_isLocator(false),
m_blendEquation(-1),
m_externalTransform(cocos2d::AffineTransformMakeIdentity()),
m_childTransform(cocos2d::AffineTransformMakeIdentity()),
m_atlasScale(1.0f)
{

}

void GAFSprite::setExternaTransform(const cocos2d::AffineTransform& transform)
{
    if (!cocos2d::AffineTransformEqualToTransform(m_externalTransform, transform))
    {
        m_externalTransform = transform;
        _transformDirty = true;
        _inverseDirty = true;
    }
}

void GAFSprite::setAtlasScale(float scale)
{
    if (m_atlasScale != scale)
    {
        m_atlasScale = scale;
        _transformDirty = true;
        _inverseDirty = true;
    }
}



const cocos2d::Mat4& GAFSprite::getNodeToParentTransform() const
{
    if (_transformDirty)
    {
        cocos2d::AffineTransform trans = m_externalTransform;
        if (m_atlasScale != 1.f)
        {
            trans = cocos2d::AffineTransformScale(trans, m_atlasScale, m_atlasScale);
        }

        cocos2d::CGAffineToGL(cocos2d::AffineTransformTranslate(trans, -_anchorPointInPoints.x, -_anchorPointInPoints.y), _transform.m);
        _transformDirty = false;
    }

    return _transform;
}


cocos2d::AffineTransform GAFSprite::getNodeToParentAffineTransform() const
{
    cocos2d::AffineTransform t;
    if (_transformDirty)
    {
        t = m_externalTransform;
        if (m_atlasScale != 1.0f)
        {
            t = cocos2d::AffineTransformScale(t, m_atlasScale, m_atlasScale);
        }
            
        cocos2d::CGAffineToGL(cocos2d::AffineTransformTranslate(t, -_anchorPointInPoints.x, -_anchorPointInPoints.y), _transform.m);
        _transformDirty = false;
    }

    cocos2d::GLToCGAffine(_transform.m, &t);

    return t;
}

#if COCOS2D_VERSION < 0x00030200
void GAFSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
#else
void GAFSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
#endif
{
#if 1
    if (m_isLocator)
    {
        return;
    }

    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "GAFSprite - draw");

    CCAssert(!_batchNode, "If cocos2d::Sprite is being rendered by CCSpriteBatchNode, cocos2d::Sprite#draw SHOULD NOT be called");

    CC_NODE_DRAW_SETUP();

    if (m_useSeparateBlendFunc)
    {
        glBlendFuncSeparate(m_blendFuncSeparate.src, m_blendFuncSeparate.dst,
            m_blendFuncSeparate.srcAlpha, m_blendFuncSeparate.dstAlpha);
    }
    else
    {
       cocos2d::GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    }

    if (m_blendEquation != -1)
    {
        glBlendEquation(m_blendEquation);
    }

    if (_texture != NULL)
    {
        cocos2d::GL::bindTexture2D(_texture->getName());
    }
    else
    {
        cocos2d::GL::bindTexture2D(0);
    }

    //
    // Attributes
    //

    cocos2d::GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    setUniformsForFragmentShader();
    CHECK_GL_ERROR_DEBUG();

#define kQuadSize sizeof(_quad.bl)
    long offset = (long)&_quad;

    // vertex
    int diff = offsetof(cocos2d::V3F_C4B_T2F, vertices);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

    // texCoods
    diff = offsetof(cocos2d::V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

    // color
    diff = offsetof(cocos2d::V3F_C4B_T2F, colors);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    CHECK_GL_ERROR_DEBUG();

#if 0 // TODO: port it on ccx 3

#if CC_SPRITE_DEBUG_DRAW == 1
    // draw bounding box
    cocos2d::Vect vertices[4] = {
        cocos2d::Vect(m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y),
        cocos2d::Vect(m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y),
        cocos2d::Vect(m_sQuad.br.vertices.x, m_sQuad.br.vertices.y),
        cocos2d::Vect(m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y),
    };
    ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
    // draw texture box
    cocos2d::Size s = this->getTextureRect().size;
    cocos2d::Vect offsetPix = this->getOffsetPosition();
    cocos2d::Vect vertices[4] = {
        cocos2d::Vect(offsetPix.x, offsetPix.y), cocos2d::Vect(offsetPix.x + s.width, offsetPix.y),
        cocos2d::Vect(offsetPix.x + s.width, offsetPix.y + s.height), cocos2d::Vect(offsetPix.x, offsetPix.y + s.height)
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

#endif

    cocos2d::CC_INCREMENT_GL_DRAWS(1);

    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "GAFSprite - draw");
#else
    CC_NODE_DRAW_SETUP();
    setUniformsForFragmentShader();
    m_quadCommand.init(_globalZOrder, _texture->getName(), _shaderProgram, _blendFunc, &_quad, 1, _transform);
    renderer->addCommand(&m_quadCommand);

    //cocos2d::Sprite::draw(renderer, _transform, true);
    
#endif
}

void GAFSprite::invalidateTransformCache()
{
    _transformDirty = true;
}

void GAFSprite::setUniformsForFragmentShader()
{

}

bool GAFSprite::initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated)
{
    if (cocos2d::Sprite::initWithTexture(pTexture, rect, rotated))
    {
        setGLProgram(cocos2d::ShaderCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
        return true;
    }
    else
    {
        return false;
    }
}

void GAFSprite::setTexture(cocos2d::Texture2D *texture)
{
    // If batchnode, then texture id should be the same
    CCAssert(!_batchNode || texture->getName() == _batchNode->getTexture()->getName(), "cocos2d::Sprite: Batched sprites should use the same texture as the batchnode");
    // accept texture==nil as argument
    CCAssert(!texture || dynamic_cast<cocos2d::Texture2D*>(texture), "setTexture expects a cocos2d::Texture2D. Invalid argument");

    if (!_batchNode && _texture != texture)
    {
        CC_SAFE_RETAIN(texture);
        CC_SAFE_RELEASE(_texture);
        _texture = texture;
        updateBlendFunc();
    }
}

bool GAFSprite::initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
    CCASSERT(spriteFrame != nullptr, "");

    bool bRet = cocos2d::Sprite::initWithTexture(spriteFrame->getTexture(), spriteFrame->getRect());
    setSpriteFrame(spriteFrame);

    return bRet;
}