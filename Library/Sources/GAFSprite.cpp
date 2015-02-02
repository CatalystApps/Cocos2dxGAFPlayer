#include "GAFPrecompiled.h"
#include "GAFSprite.h"
#include "GAFCollections.h"

#include "math/TransformUtils.h"
#include "../external/xxhash/xxhash.h"

USING_NS_CC;

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__A__) ( (int)(__A__))
#endif

NS_GAF_BEGIN

GAFSprite::GAFSprite()
:
objectIdRef(IDNONE),
m_useSeparateBlendFunc(false),
m_isLocator(false),
m_blendEquation(-1),
m_atlasScale(1.0f),
m_externalTransform(AffineTransform::IDENTITY)
{
#if COCOS2D_VERSION < 0x00030300
    _batchNode = nullptr; // this will fix a bug in cocos2dx 3.2 tag
#endif
    setFlippedX(false); // Fix non-inited vars in cocos
    setFlippedY(false);
    _rectRotated = false;
}

bool GAFSprite::initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
    CCASSERT(spriteFrame != nullptr, "");

    bool bRet = cocos2d::Sprite::initWithTexture(spriteFrame->getTexture(), spriteFrame->getRect());
    setSpriteFrame(spriteFrame);

    return bRet;
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

void GAFSprite::setExternalTransform(const cocos2d::AffineTransform& transform)
{
    if (!cocos2d::AffineTransformEqualToTransform(getExternalTransform(), transform))
    {
        m_externalTransform = transform;
        _transformUpdated = true;
        _transformDirty = true;
        _inverseDirty = true;
    }
}

const cocos2d::AffineTransform& GAFSprite::getExternalTransform() const
{
    return m_externalTransform;
}

const cocos2d::Mat4& GAFSprite::getNodeToParentTransform() const
{
    if (_transformDirty)
    {
        if (m_atlasScale != 1.f)
        {
            cocos2d::AffineTransform transform = cocos2d::AffineTransformScale(getExternalTransform(), m_atlasScale, m_atlasScale);
            cocos2d::CGAffineToGL(cocos2d::AffineTransformTranslate(transform, -_anchorPointInPoints.x, -_anchorPointInPoints.y), _transform.m);
        }
        else
        {
            cocos2d::CGAffineToGL(cocos2d::AffineTransformTranslate(getExternalTransform(), -_anchorPointInPoints.x, -_anchorPointInPoints.y), _transform.m);
        }
        _transformDirty = false;
    }

    return _transform;
}

cocos2d::AffineTransform GAFSprite::getNodeToParentAffineTransform() const
{
    cocos2d::AffineTransform transform;
    if (_transformDirty)
    {
        transform = getExternalTransform();
        if (m_atlasScale != 1.0f)
        {
            transform = cocos2d::AffineTransformScale(transform, m_atlasScale, m_atlasScale);
        }

        cocos2d::CGAffineToGL(cocos2d::AffineTransformTranslate(transform, -_anchorPointInPoints.x, -_anchorPointInPoints.y), _transform.m);
        _transformDirty = false;
    }
    cocos2d::GLToCGAffine(_transform.m, &transform);

    return transform;
}

#if COCOS2D_VERSION < 0x00030200
void GAFSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
{
    (void)transformUpdated;
#else
void GAFSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    (void)flags;
#endif
    if (m_isLocator)
    {
        return;
    }

    _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
    if (!_insideBounds)
        return;

    uint32_t id = setUniforms();

    if (m_useSeparateBlendFunc || (m_blendEquation != -1))
    {
        m_customCommand.init(_globalZOrder);
        m_customCommand.func = CC_CALLBACK_0(GAFSprite::customDraw, this, transform);
        renderer->addCommand(&m_customCommand);
    }
    else
    {
        m_quad = _quad;

        transform.transformPoint(&m_quad.tl.vertices);
        transform.transformPoint(&m_quad.tr.vertices);
        transform.transformPoint(&m_quad.bl.vertices);
        transform.transformPoint(&m_quad.br.vertices);

        m_quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &m_quad, 1, Mat4::IDENTITY, id);
        renderer->addCommand(&m_quadCommand);
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

uint32_t GAFSprite::setUniforms()
{
#if COCOS2D_VERSION < 0x00030300
    uint32_t materialID = QuadCommand::MATERIAL_ID_DO_NOT_BATCH;
#else
    uint32_t materialID = Renderer::MATERIAL_ID_DO_NOT_BATCH;
#endif
    if (_glProgramState->getUniformCount() == 0)
    {
        int glProgram = (int)getGLProgram()->getProgram();
        int intArray[4] = { glProgram, (int)getTexture()->getName(), (int)getBlendFunc().src, (int)getBlendFunc().dst };

        materialID = XXH32((const void*)intArray, sizeof(intArray), 0);
    }
    return materialID;
}

void GAFSprite::customDraw(cocos2d::Mat4& transform)
{
    CCAssert(!_batchNode, "If cocos2d::Sprite is being rendered by CCSpriteBatchNode, cocos2d::Sprite#draw SHOULD NOT be called");

    getGLProgramState()->apply(transform);

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

    //USING_NS_CC;
    //CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
    //CC_INCREMENT_GL_DRAWS(1);
}

NS_GAF_END
