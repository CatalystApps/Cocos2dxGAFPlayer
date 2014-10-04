#include "GAFPrecompiled.h"
#include "GAFMovieClip.h"

#include "GAFShaderManager.h"

#include "GAFSubobjectState.h"

#include "GAFFilterData.h"
#include "GAFFilterManager.h"

#include "../external/xxhash/xxhash.h"


#define CHECK_CTX_IDENTITY 0

static int colorTransformMultLocation = -1;
static int colorTransformOffsetLocation = -1;
static int fragmentAlphaLocation = -1;
static int colorMatrixLocation = -1;
static int colorMatrixLocation2 = -1;


GAFMovieClip::GAFMovieClip():
m_colorMatrixFilterData(nullptr),
m_glowFilterData(nullptr),
m_blurFilterData(nullptr),
m_initialTexture(nullptr)
{
    m_charType = GAFCharacterType::Texture;
}

GAFMovieClip::~GAFMovieClip()
{
    CC_SAFE_RELEASE(m_initialTexture);
}

void GAFMovieClip::_setBlendingFunc()
{
    setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);
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
        cocos2d::Texture2D * resultTex = NULL;

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
#ifndef ENABLE_NEW_UNIFORM_SETTER
#define ENABLE_NEW_UNIFORM_SETTER 0
#endif
    struct Hash
    {
        int       program;
        uint32_t  texture;
        cocos2d::BlendFunc blend;
        decltype(m_colorTransformMult)      a;
        decltype(m_colorTransformOffsets)   b;
        decltype(m_colorTransformMult.w)    c;
        cocos2d::Mat4    d;
        cocos2d::Vec4    e;
        cocos2d::Mat4    f;
        cocos2d::Vec4    g;
    };

    cocos2d::GLProgramState* state = getGLProgramState();

    Hash hash;
    memset(&hash, 0, sizeof(Hash));

    hash.program = getGLProgram()->getProgram();
    hash.texture = _texture->getName();
    hash.blend = _blendFunc;

    bool usingColorTransform = (colorTransformMultLocation > -1) && (colorTransformOffsetLocation > -1);
    bool usingFragmentAlpha = (!usingColorTransform) && (fragmentAlphaLocation > -1);
    bool usingColorMatrix = (colorMatrixLocation > -1 && colorMatrixLocation2 > -1);
    bool usingColorMatrixWithFilter = usingColorMatrix && m_colorMatrixFilterData;
    usingColorMatrix = usingColorMatrix && !m_colorMatrixFilterData;

    if (usingColorTransform)
    {
#if ENABLE_NEW_UNIFORM_SETTER
        state->setUniformVec4(colorTransformMultLocation, m_colorTransformMult);
        state->setUniformVec4(colorTransformOffsetLocation, m_colorTransformOffsets);
#else
        state->setUniformVec4("colorTransformMult", m_colorTransformMult);
        state->setUniformVec4("colorTransformOffsets", m_colorTransformOffsets);
#endif
        hash.a = m_colorTransformMult;
        hash.b = m_colorTransformOffsets;
    }

    if (usingFragmentAlpha)
    {
#if ENABLE_NEW_UNIFORM_SETTER
        state->setUniformFloat(fragmentAlphaLocation, m_colorTransformMult.w);
#else
        state->setUniformFloat("fragmentAlpha", m_colorTransformMult.w);
#endif
        hash.c = m_colorTransformMult.w;
    }

    if (usingColorMatrix)
    {
#if ENABLE_NEW_UNIFORM_SETTER
        state->setUniformMat4(colorMatrixLocation, m_colorMatrixIdentity1);
        state->setUniformVec4(colorMatrixLocation2, m_colorMatrixIdentity2);
#else
        state->setUniformMat4("colorMatrix", m_colorMatrixIdentity1);
        state->setUniformVec4("colorMatrix2", m_colorMatrixIdentity2);
#endif
        hash.d = m_colorMatrixIdentity1;
        hash.e = m_colorMatrixIdentity2;
    }

    if (usingColorMatrixWithFilter)
    {
#if ENABLE_NEW_UNIFORM_SETTER
        state->setUniformMat4(colorMatrixLocation, Mat4(m_colorMatrixFilterData->matrix));
        state->setUniformVec4(colorMatrixLocation2, Vec4(m_colorMatrixFilterData->matrix2));
#else
        state->setUniformMat4("colorMatrix", cocos2d::Mat4(m_colorMatrixFilterData->matrix));
        state->setUniformVec4("colorMatrix2", cocos2d::Vec4(m_colorMatrixFilterData->matrix2));
#endif
        hash.f = cocos2d::Mat4(m_colorMatrixFilterData->matrix);
        hash.g = cocos2d::Vec4(m_colorMatrixFilterData->matrix2);
    }

    return XXH32((void*)&hash, sizeof(Hash), 0);
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
        setGLProgramState(cocos2d::GLProgramState::create(programForShader()));
        return true;
    }
    else
    {
        return false;
    }
}

cocos2d::GLProgram * GAFMovieClip::programForShader(bool reset /*= false*/)
{
#if CHECK_CTX_IDENTITY
    const bool isCTXidt = isCTXIdentity();
#else
    const bool isCTXidt = false;
#endif

    cocos2d::GLProgram* program = nullptr;
    if (isCTXidt)
    {
        program = cocos2d::GLProgramCache::getInstance()->getGLProgram(kGAFSpriteWithAlphaShaderProgramCache_noCTX);
    }
    else
    {
        program = cocos2d::GLProgramCache::getInstance()->getGLProgram(kGAFSpriteWithAlphaShaderProgramCacheKey);
    }

    if (program && reset)
    {
        program->reset();
    }

    if (!program || reset)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && !defined(_DEBUG))
#include "ShadersPrecompiled/GAFPrecompiledShaders.h"
        //program = cocos2d::GLProgram::addP;
        program = cocos2d::GLProgram::createWithPrecompiledProgramByteArray(kGAFScrollLayerAlphaFilterProgramCacheKey, kGAFScrollLayerAlphaFilterProgramCacheKey);
        program->addAttribute(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
        program->addAttribute(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
        program->addAttribute(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        program->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        cocos2d::GLProgramCache::getInstance()->addGLProgram(program, kGAFSpriteWithAlphaShaderProgramCacheKey);
#else
        const char* fragmentShader = nullptr;
        if (isCTXidt)
        {
            fragmentShader = GAFShaderManager::getShader(GAFShaderManager::EFragmentShader::AlphaNoCtx);
        }
        else
        {
            fragmentShader = GAFShaderManager::getShader(GAFShaderManager::EFragmentShader::Alpha);
        }
        program = cocos2d::GLProgram::createWithByteArrays(cocos2d::ccPositionTextureColor_vert, fragmentShader);

        if (program)
        {
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
            //program->link();
            //program->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
            if (isCTXidt)
            {
                cocos2d::ShaderCache::getInstance()->addGLProgram(program, kGAFSpriteWithAlphaShaderProgramCache_noCTX);
            }
            else
            {
                cocos2d::ShaderCache::getInstance()->addGLProgram(program, kGAFSpriteWithAlphaShaderProgramCacheKey);
            }
        }
        else
        {
            CCLOGERROR("Cannot load program for GAFMovieClip.");
            CC_SAFE_DELETE(program);
            return NULL;
        }

        CHECK_GL_ERROR_DEBUG();
#endif
        program->use();
        colorTransformMultLocation = glGetUniformLocation(program->getProgram(), "colorTransformMult");
        colorTransformOffsetLocation = glGetUniformLocation(program->getProgram(), "colorTransformOffsets");
        colorMatrixLocation = glGetUniformLocation(program->getProgram(), "colorMatrix");
        colorMatrixLocation2 = glGetUniformLocation(program->getProgram(), "colorMatrix2");
        fragmentAlphaLocation = glGetUniformLocation(program->getProgram(), "fragmentAlpha");
    }
    return program;
}

void GAFMovieClip::setColorTransform(const cocos2d::Vec4 &mults, const cocos2d::Vec4 &offsets)
{
    m_colorTransformMult = mults;
    m_colorTransformOffsets = offsets;
    _setBlendingFunc();
#if CHECK_CTX_IDENTITY
    setShaderProgram(programForShader());
#endif
}

void GAFMovieClip::setColorTransform(const GLfloat * mults, const GLfloat * offsets)
{
    m_colorTransformMult = cocos2d::Vec4(mults);
    m_colorTransformOffsets = cocos2d::Vec4(offsets);
    _setBlendingFunc();
#if CHECK_CTX_IDENTITY
    setShaderProgram(programForShader());
#endif
}

void GAFMovieClip::setColorTransform(const GLfloat * colorTransform)
{
    m_colorTransformMult = cocos2d::Vec4(colorTransform);
    m_colorTransformOffsets = cocos2d::Vec4(&colorTransform[4]);

    _setBlendingFunc();
#if CHECK_CTX_IDENTITY
    setShaderProgram(programForShader());
#endif
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

bool GAFMovieClip::isCTXIdentity() const
{
    if (m_colorTransformMult != cocos2d::Vec4::ONE)
    {
        return false;
    }
    if (m_colorTransformOffsets != cocos2d::Vec4::ZERO)
    {
        return false;
    }
    return true;
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