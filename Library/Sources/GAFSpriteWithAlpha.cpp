#include "GAFPrecompiled.h"
#include "GAFSpriteWithAlpha.h"
#include "GAFShaderManager.h"

#include "GAFSubobjectState.h"

#include "GAFSpriteWithAlpha.h"
#include "GAFFilterData.h"
#include "GAFFilterManager.h"

#include "../external/xxhash/xxhash.h"

USING_NS_CC;

NS_GAF_BEGIN

struct GAFSpriteWithAlphaHash
{
    int       program;
    uint32_t  texture;
    BlendFunc blend;
    cocos2d::Vec4   a;
    cocos2d::Vec4   b;
    float   c;
    Mat4    d;
    Vec4    e;        
};
    
GAFSpriteWithAlpha::GAFSpriteWithAlpha()
:
m_initialTexture(nullptr),
m_colorMatrixFilterData(nullptr),
m_glowFilterData(nullptr),
m_blurFilterData(nullptr),
m_programBase(nullptr),
m_programNoCtx(nullptr),
m_hasCtx(false)
{
}

GAFSpriteWithAlpha::~GAFSpriteWithAlpha()
{
    CC_SAFE_RELEASE(m_initialTexture);
    _glProgramState = nullptr; // Should be treated here as weak pointer
    CC_SAFE_RELEASE(m_programBase);
    CC_SAFE_RELEASE(m_programNoCtx);

}

bool GAFSpriteWithAlpha::initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated)
{
    if (GAFSprite::initWithTexture(pTexture, rect, rotated))
    {
        m_initialTexture = pTexture;
        m_initialTexture->retain();
        m_initialTextureRect = rect;
        m_colorTransformMult = cocos2d::Vec4::ONE;
        m_colorTransformOffsets = cocos2d::Vec4::ZERO;
        _setBlendingFunc();

        m_programBase = GLProgramState::create(GAFShaderManager::getProgram(GAFShaderManager::EPrograms::Alpha));
        m_programBase->retain();
#if CHECK_CTX_IDENTITY
        m_programNoCtx = GLProgramState::create(GAFShaderManager::getProgram(GAFShaderManager::EPrograms::AlphaNoCtx));
        m_programNoCtx->retain();
#endif
#if CHECK_CTX_IDENTITY
        _glProgramState = m_programNoCtx;
#else
        _glProgramState = m_programBase;
#endif
        return true;
    }
    else
    {
        return false;
    }
}

void GAFSpriteWithAlpha::updateTextureWithEffects()
{
    if (!m_blurFilterData && !m_glowFilterData)
    {
        setTexture(m_initialTexture);
        setTextureRect(m_initialTextureRect, false, m_initialTextureRect.size);
        setFlippedY(false);
    }
    else
    {
        cocos2d::Texture2D * resultTex = nullptr;

        if (m_blurFilterData)
        {
            resultTex = GAFFilterManager::getInstance()->applyFilter(Sprite::createWithTexture(m_initialTexture, m_initialTextureRect), m_blurFilterData);
        }
        else if (m_glowFilterData)
        {
            resultTex = GAFFilterManager::getInstance()->applyFilter(Sprite::createWithTexture(m_initialTexture, m_initialTextureRect), m_glowFilterData);
        }

        if (resultTex)
        {
            setTexture(resultTex);
            setFlippedY(true);
            cocos2d::Rect texureRect = cocos2d::Rect(0, 0, resultTex->getContentSize().width, resultTex->getContentSize().height);
            setTextureRect(texureRect, false, texureRect.size);
        }
    }
}

uint32_t GAFSpriteWithAlpha::setUniforms()
{
#if GAF_ENABLE_NEW_UNIFORM_SETTER
#define getUniformId(x) GAFShaderManager::getUniformLocation(x)
#else
#define getUniformId(x) GAFShaderManager::getUniformName(x)
#endif
    
#if CHECK_CTX_IDENTITY
    const bool ctx = hasCtx();
#else
    const bool ctx = false;
#endif


    GLProgramState* state = getGLProgramState();
    
    GAFSpriteWithAlphaHash hash;
    memset(&hash, 0, sizeof(GAFSpriteWithAlphaHash));
    
    hash.program = getGLProgram()->getProgram();
    hash.texture = _texture->getName();
    hash.blend = _blendFunc;

    
    if (!ctx)
    {        
        hash.c = m_colorTransformMult.w;
        state->setUniformFloat(
            getUniformId(GAFShaderManager::EUniforms::Alpha),
            m_colorTransformMult.w);
    }
    else
    {
        {
            hash.a = m_colorTransformMult;
            hash.b = m_colorTransformOffsets;
            state->setUniformVec4(
                getUniformId(GAFShaderManager::EUniforms::ColorTransformMult),
                m_colorTransformMult);
            state->setUniformVec4(
                getUniformId(GAFShaderManager::EUniforms::ColorTransformOffset),
                m_colorTransformOffsets);
        }

        if (!m_colorMatrixFilterData)
        {
            hash.d = m_colorMatrixIdentity1;
            hash.e = m_colorMatrixIdentity2;
            state->setUniformMat4(
                getUniformId(GAFShaderManager::EUniforms::ColorMatrixBody),
                m_colorMatrixIdentity1);
            state->setUniformVec4(
                getUniformId(GAFShaderManager::EUniforms::ColorMatrixAppendix),
                m_colorMatrixIdentity2);
        }
        else
        {
            hash.d = Mat4(m_colorMatrixFilterData->matrix);
            hash.e = Vec4(m_colorMatrixFilterData->matrix2);
            state->setUniformMat4(
                getUniformId(GAFShaderManager::EUniforms::ColorMatrixBody),
                Mat4(m_colorMatrixFilterData->matrix));
            state->setUniformVec4(
                getUniformId(GAFShaderManager::EUniforms::ColorMatrixAppendix),
                Vec4(m_colorMatrixFilterData->matrix2));
        }
    }
    return XXH32((void*)&hash, sizeof(GAFSpriteWithAlphaHash), 0);
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * mults, const GLfloat * offsets)
{
    m_colorTransformMult = Vec4(mults);
    m_colorTransformOffsets = Vec4(offsets);
    _setBlendingFunc();
    m_ctxDirty = true;
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * colorTransform)
{
    m_colorTransformMult = Vec4(colorTransform);
    m_colorTransformOffsets = Vec4(&colorTransform[4]);
    _setBlendingFunc();
    m_ctxDirty = true;
}

void GAFSpriteWithAlpha::_setBlendingFunc()
{
    setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);
}

void GAFSpriteWithAlpha::setColorMarixFilterData( GAFColorColorMatrixFilterData* data )
{
    m_colorMatrixFilterData = data;
}

void GAFSpriteWithAlpha::setGlowFilterData( GAFGlowFilterData* data )
{
    if (m_glowFilterData != data)
    {
        m_glowFilterData = data;
        updateTextureWithEffects();
    }
}

void GAFSpriteWithAlpha::setBlurFilterData( GAFBlurFilterData* data )
{
    if (m_blurFilterData != data)
    {
        m_blurFilterData = data;
        updateTextureWithEffects();
    }
}

cocos2d::Texture2D* GAFSpriteWithAlpha::getInitialTexture() const
{
    return m_initialTexture;
}

const cocos2d::Rect& GAFSpriteWithAlpha::getInitialTextureRect() const
{
    return m_initialTextureRect;
}

void GAFSpriteWithAlpha::updateCtx()
{
    m_ctxDirty = false;
    if ((m_colorTransformMult != cocos2d::Vec4::ONE) || (m_colorTransformOffsets != cocos2d::Vec4::ZERO))
    {
        _glProgramState = m_programBase;
        m_hasCtx = true;
    }
    else
    {
        _glProgramState = m_programNoCtx;
        m_hasCtx = false;
    }
}

bool GAFSpriteWithAlpha::hasCtx()
{
    if (m_ctxDirty)
        updateCtx();

    return _glProgramState == m_programBase;
}

NS_GAF_END
