#include "GAFPrecompiled.h"
#include "GAFMovieClip.h"

#include "GAFShaderManager.h"

#include "GAFSubobjectState.h"

#include "GAFFilterData.h"
#include "GAFFilterManager.h"

#include "../external/xxhash/xxhash.h"

USING_NS_CC;

NS_GAF_BEGIN

struct GAFMovieClipHash
{
    int       program;
    uint32_t  texture;
    BlendFunc blend;
    Vec4    a;
    Vec4    b;
    float   c;
    Mat4    d;
    Vec4    e;        
};
    

GAFMovieClip::GAFMovieClip():
m_initialTexture(nullptr),
m_colorMatrixFilterData(nullptr),
m_glowFilterData(nullptr),
m_blurFilterData(nullptr),
m_programBase(nullptr),
m_programNoCtx(nullptr),
m_ctxDirty(false)
{
    m_charType = GAFCharacterType::Texture;
}

GAFMovieClip::~GAFMovieClip()
{
    CC_SAFE_RELEASE(m_initialTexture);
    _glProgramState = nullptr; // Should be treated here as weak pointer
    CC_SAFE_RELEASE(m_programBase);
    CC_SAFE_RELEASE(m_programNoCtx);
}

bool GAFMovieClip::initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated)
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

void GAFMovieClip::updateTextureWithEffects()
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

uint32_t GAFMovieClip::setUniforms()
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
    
    GAFMovieClipHash hash;
    memset(&hash, 0, sizeof(GAFMovieClipHash));
    
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
    return XXH32((void*)&hash, sizeof(GAFMovieClipHash), 0);
}

void GAFMovieClip::setColorTransform(const GLfloat * mults, const GLfloat * offsets)
{
    m_colorTransformMult = Vec4(mults);
    m_colorTransformOffsets = Vec4(offsets);
    _setBlendingFunc();
    m_ctxDirty = true;
}

void GAFMovieClip::setColorTransform(const GLfloat * colorTransform)
{
    m_colorTransformMult = Vec4(colorTransform);
    m_colorTransformOffsets = Vec4(&colorTransform[4]);
    _setBlendingFunc();
    m_ctxDirty = true;
}

void GAFMovieClip::_setBlendingFunc()
{
    setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);
}

void GAFMovieClip::setColorMarixFilterData(GAFColorColorMatrixFilterData* data)
{
    m_colorMatrixFilterData = data;
}

void GAFMovieClip::setGlowFilterData(GAFGlowFilterData* data)
{
    if (m_glowFilterData != data)
    {
        m_glowFilterData = data;
        updateTextureWithEffects();
    }
}

void GAFMovieClip::setBlurFilterData(GAFBlurFilterData* data)
{
    if (m_blurFilterData != data)
    {
        m_blurFilterData = data;
        updateTextureWithEffects();
    }
}

cocos2d::Texture2D* GAFMovieClip::getInitialTexture() const
{
    return m_initialTexture;
}

const cocos2d::Rect& GAFMovieClip::getInitialTextureRect() const
{
    return m_initialTextureRect;
}

void GAFMovieClip::updateCtx()
{
    m_ctxDirty = false;
    if ((m_colorTransformMult != cocos2d::Vec4::ONE) || (m_colorTransformOffsets != cocos2d::Vec4::ZERO))
    {
        _glProgramState = m_programBase;
    }
    else
    {
        _glProgramState = m_programNoCtx;
    }
}

bool GAFMovieClip::hasCtx()
{
    if (m_ctxDirty)
        updateCtx();

    return _glProgramState == m_programBase;
}

#if COCOS2D_VERSION < 0x00030200
void GAFMovieClip::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
{
    GAFSprite::draw(renderer, transform, transformUpdated);
#else
void GAFMovieClip::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
#endif
    GAFSprite::draw(renderer, transform, flags);

}

NS_GAF_END