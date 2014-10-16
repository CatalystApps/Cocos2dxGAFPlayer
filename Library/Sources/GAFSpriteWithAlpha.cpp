#include "GAFPrecompiled.h"
#include "GAFSpriteWithAlpha.h"
#include "GAFShaderManager.h"

#include "GAFSubobjectState.h"

#include "GAFSpriteWithAlpha.h"
#include "GAFFilterData.h"
#include "GAFFilterManager.h"

#include "../external/xxhash/xxhash.h"

USING_NS_CC;

#ifndef GAF_ENABLE_NEW_UNIFORM_SETTER
// Fast uniform setter is available since v3.2
#define GAF_ENABLE_NEW_UNIFORM_SETTER COCOS2D_VERSION >= 0x00030200
#endif

#define CHECK_CTX_IDENTITY 1

namespace gaf
{
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

cocos2d::GLProgram * GAFSpriteWithAlpha::programForShader(bool reset)
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
            CCLOGERROR("Cannot load program for GAFSpriteWithAlpha.");
            CC_SAFE_DELETE(program);
            return nullptr;
        }

        CHECK_GL_ERROR_DEBUG();
        program->use();/*
        if(isCTXidt)
        {
            fragmentAlphaLocation = glGetUniformLocation(program->getProgram(), "fragmentAlpha");
        }
        else
        {
            colorTransformMultLocation = glGetUniformLocation(program->getProgram(), "colorTransformMult");
            colorTransformOffsetLocation = glGetUniformLocation(program->getProgram(), "colorTransformOffsets");
            colorMatrixLocation = glGetUniformLocation(program->getProgram(), "colorMatrix");
            colorMatrixLocation2 = glGetUniformLocation(program->getProgram(), "colorMatrix2");
        }*/
    }
    return program;
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
#if CHECK_CTX_IDENTITY
    const bool isCTXidt = isCTXIdentity();
#else
    const bool isCTXidt = false;
#endif


    GLProgramState* state = getGLProgramState();
    
    GAFSpriteWithAlphaHash hash;
    memset(&hash, 0, sizeof(GAFSpriteWithAlphaHash));
    
    hash.program = getGLProgram()->getProgram();
    hash.texture = _texture->getName();
    hash.blend = _blendFunc;

    
    if(isCTXidt)
    {
#if GAF_ENABLE_NEW_UNIFORM_SETTER
        state->setUniformFloat(
            GAFShaderManager::getUniformLocation(GAFShaderManager::EUniforms::Alpha),
            m_colorTransformMult.w);
#else
        state->setUniformFloat("fragmentAlpha", m_colorTransformMult.w);
#endif
        hash.c = m_colorTransformMult.w;
    }
    else
    {
        {
#if GAF_ENABLE_NEW_UNIFORM_SETTER
            state->setUniformVec4(
                GAFShaderManager::getUniformLocation(GAFShaderManager::EUniforms::ColorTransformMult),
                m_colorTransformMult);
            state->setUniformVec4(
                GAFShaderManager::getUniformLocation(GAFShaderManager::EUniforms::ColorTransformOffset),
                m_colorTransformOffsets);
#else
            state->setUniformVec4("colorTransformMult", m_colorTransformMult);
            state->setUniformVec4("colorTransformOffsets", m_colorTransformOffsets);
#endif
            hash.a = m_colorTransformMult;
            hash.b = m_colorTransformOffsets;
        }

        if (!m_colorMatrixFilterData)
        {
#if GAF_ENABLE_NEW_UNIFORM_SETTER
            state->setUniformMat4(
                GAFShaderManager::getUniformLocation(GAFShaderManager::EUniforms::ColorMatrixBody),
                m_colorMatrixIdentity1);
            state->setUniformVec4(
                GAFShaderManager::getUniformLocation(GAFShaderManager::EUniforms::ColorMatrixAppendix), 
                m_colorMatrixIdentity2);
#else
            state->setUniformMat4("colorMatrix", m_colorMatrixIdentity1);
            state->setUniformVec4("colorMatrix2", m_colorMatrixIdentity2);
#endif
            hash.d = m_colorMatrixIdentity1;
            hash.e = m_colorMatrixIdentity2;
        }
        else
        {
#if GAF_ENABLE_NEW_UNIFORM_SETTER
            state->setUniformMat4(
                GAFShaderManager::getUniformLocation(GAFShaderManager::EUniforms::ColorMatrixBody), 
                Mat4(m_colorMatrixFilterData->matrix));
            state->setUniformVec4(
                GAFShaderManager::getUniformLocation(GAFShaderManager::EUniforms::ColorMatrixAppendix), 
                Vec4(m_colorMatrixFilterData->matrix2));
#else
            state->setUniformMat4("colorMatrix", Mat4(m_colorMatrixFilterData->matrix));
            state->setUniformVec4("colorMatrix2", Vec4(m_colorMatrixFilterData->matrix2));
#endif
            hash.d = Mat4(m_colorMatrixFilterData->matrix);
            hash.e = Vec4(m_colorMatrixFilterData->matrix2);
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
#if CHECK_CTX_IDENTITY
    if (isCTXIdentity())
    {
        _glProgramState = m_programNoCtx;
    }
    else
    {
        _glProgramState = m_programBase;
    }
#endif
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * colorTransform)
{
    m_colorTransformMult = Vec4(colorTransform);
    m_colorTransformOffsets = Vec4(&colorTransform[4]);
    _setBlendingFunc();
    m_ctxDirty = true;
#if CHECK_CTX_IDENTITY
    if (isCTXIdentity())
    {
        _glProgramState = m_programNoCtx;
    }
    else
    {
        _glProgramState = m_programBase;
    }
#endif
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
    bool newCtx;
    if ((m_colorTransformMult != cocos2d::Vec4::ONE) || (m_colorTransformOffsets != cocos2d::Vec4::ZERO))
    {
        newCtx = true;
    }
    else
    {
        newCtx = false;
    }
    m_hasCtx = newCtx;
}

bool GAFSpriteWithAlpha::isCTXIdentity()
{
    if (m_ctxDirty)
        updateCtx();

    return !m_hasCtx;
}

#if 0 // CC_ENABLE_CACHE_TEXTURE_DATA
void _GAFreloadAlphaShader()
{
    cocos2d::GLProgram * program = cocos2d::ShaderCache::getInstance()->getProgram(kGAFSpriteWithAlphaShaderProgramCacheKey);

    if (!program)
    {
        return;
    }
    program->reset();
    program = GAFShaderManager::createWithFragmentFilename(ccPositionTextureColor_vert, kAlphaFragmentShaderFilename, program);
    if (program)
    {
        program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        program->link();
        program->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        program->use();
        _colorTrasformLocation = (GLuint)glGetUniformLocation(program->getProgram(), "colorTransform");
        if (_colorTrasformLocation <= 0)
        {
            CCAssert(false, "Can not RELOAD GAFSpriteWithAlpha");
        }
        CCLOGERROR("GAFSpriteWithAlpha RELOADED");
    }
    else
    {
        CCAssert(false, "Can not RELOAD GAFSpriteWithAlpha");
    }
}
#endif

}
