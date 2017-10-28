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
: objectIdRef(IDNONE)
, m_isLocator(false)
, m_atlasScale(1.0f)
, m_externalTransform(AffineTransform::IDENTITY)
, m_rotation(GAFRotation::NONE)
{
    setFlippedX(false); // Fix non-inited vars in cocos
    setFlippedY(false);
    _rectRotated = false;
}

bool GAFSprite::initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, GAFRotation rotation)
{
    m_rotation = rotation;
    return initWithSpriteFrame(spriteFrame);
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
        setGLProgram(cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
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

void GAFSprite::setVertexRect(const cocos2d::Rect& rect)
{
    _rect = rect;
    if (m_rotation != GAFRotation::NONE)
    {
        std::swap(_rect.size.width, _rect.size.height);
    }
}

void GAFSprite::setTextureRect(const cocos2d::Rect& rect, bool rotated, const cocos2d::Size& untrimmedSize)
{
    cocos2d::Size rotatedSize = untrimmedSize;
    if (m_rotation != GAFRotation::NONE)
    {
        rotated = true;
        rotatedSize = cocos2d::Size(rotatedSize.height, rotatedSize.width);
    }
    cocos2d::Sprite::setTextureRect(rect, rotated, rotatedSize);
}

void GAFSprite::setTextureCoords(const cocos2d::Rect& rect)
{
    cocos2d::Sprite::setTextureCoords(rect, &_quad);
}

void GAFSprite::setTextureCoords(const cocos2d::Rect& rect, cocos2d::V3F_C4B_T2F_Quad* outQuad)
{
    cocos2d::Rect rectInPixels = CC_RECT_POINTS_TO_PIXELS(rect);
    
    cocos2d::Texture2D *tex = _batchNode ? _textureAtlas->getTexture() : _texture;
    if (!tex)
    {
        return;
    }
    
    float atlasWidth = (float)tex->getPixelsWide();
    float atlasHeight = (float)tex->getPixelsHigh();
    
    float rw = rectInPixels.size.width;
    float rh = rectInPixels.size.height;
    
    if (_rectRotated)
        std::swap(rw, rh);
    
    float left = rectInPixels.origin.x / atlasWidth;
    float right = (rectInPixels.origin.x + rw) / atlasWidth;
    float top = rectInPixels.origin.y / atlasHeight;
    float bottom = (rectInPixels.origin.y + rh) / atlasHeight;
    
    switch (m_rotation)
    {
        case gaf::GAFRotation::CCW_90:
        {
            if (_flippedX)
            {
                std::swap(top, bottom);
            }
            
            if (_flippedY)
            {
                std::swap(left, right);
            }
            
            outQuad->bl.texCoords.u = right;
            outQuad->bl.texCoords.v = bottom;
            outQuad->br.texCoords.u = right;
            outQuad->br.texCoords.v = top;
            outQuad->tl.texCoords.u = left;
            outQuad->tl.texCoords.v = bottom;
            outQuad->tr.texCoords.u = left;
            outQuad->tr.texCoords.v = top;
        }
            break;
            
        case gaf::GAFRotation::CW_90:
        {
            if (_flippedX)
            {
                std::swap(top, bottom);
            }
            
            if (_flippedY)
            {
                std::swap(left, right);
            }
            
            outQuad->bl.texCoords.u = left;
            outQuad->bl.texCoords.v = top;
            outQuad->br.texCoords.u = left;
            outQuad->br.texCoords.v = bottom;
            outQuad->tl.texCoords.u = right;
            outQuad->tl.texCoords.v = top;
            outQuad->tr.texCoords.u = right;
            outQuad->tr.texCoords.v = bottom;
        }
            break;
            
        case gaf::GAFRotation::NONE:
        default:
        {
            if (_flippedX)
            {
                std::swap(left, right);
            }
            
            if (_flippedY)
            {
                std::swap(top, bottom);
            }
            
            outQuad->bl.texCoords.u = left;
            outQuad->bl.texCoords.v = bottom;
            outQuad->br.texCoords.u = right;
            outQuad->br.texCoords.v = bottom;
            outQuad->tl.texCoords.u = left;
            outQuad->tl.texCoords.v = top;
            outQuad->tr.texCoords.u = right;
            outQuad->tr.texCoords.v = top;
        }
            break;
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

void GAFSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    (void)flags;
    if (m_isLocator)
    {
        return;
    }

    uint32_t id = setUniforms();

    m_quad = _quad;

    transform.transformPoint(&m_quad.tl.vertices);
    transform.transformPoint(&m_quad.tr.vertices);
    transform.transformPoint(&m_quad.bl.vertices);
    transform.transformPoint(&m_quad.br.vertices);

    m_quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &m_quad, 1, Mat4::IDENTITY, id);
    renderer->addCommand(&m_quadCommand);
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
    uint32_t materialID = Renderer::MATERIAL_ID_DO_NOT_BATCH;
    if (_glProgramState->getUniformCount() == 0)
    {
        int glProgram = (int)getGLProgram()->getProgram();
        int intArray[4] = { glProgram, (int)getTexture()->getName(), (int)getBlendFunc().src, (int)getBlendFunc().dst };

        materialID = XXH32((const void*)intArray, sizeof(intArray), 0);
    }
    return materialID;
}

NS_GAF_END
