#include "GAFPrecompiled.h"
#include "GAFSpriteWithAlpha.h"
#include "GAFShaderManager.h"

#include "GAFSubobjectState.h"

#include "GAFSpriteWithAlpha.h"
#include "GAFFilterData.h"
#include "GAFFilterManager.h"

#include "../external/xxhash/xxhash.h"

USING_NS_CC;

#define CHECK_CTX_IDENTITY 0

static int colorTransformMultLocation = -1;
static int colorTransformOffsetLocation = -1;
static int fragmentAlphaLocation = -1;
static int colorMatrixLocation   = -1;
static int colorMatrixLocation2  = -1;

GAFSpriteWithAlpha::GAFSpriteWithAlpha()
:
m_initialTexture(NULL),
m_colorMatrixFilterData(NULL),
m_glowFilterData(NULL),
m_blurFilterData(NULL)
{
}

GAFSpriteWithAlpha::~GAFSpriteWithAlpha()
{
    CC_SAFE_RELEASE(m_initialTexture);
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
        setGLProgramState(GLProgramState::create(programForShader()));
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
        program = cocos2d::ShaderCache::getInstance()->getGLProgram(kGAFSpriteWithAlphaShaderProgramCache_noCTX);
    }
    else
    {
        program = cocos2d::ShaderCache::getInstance()->getGLProgram(kGAFSpriteWithAlphaShaderProgramCacheKey);
    }

    if (program && reset)
    {
        program->reset();
    }
    
    if (!program || reset)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && !defined(_DEBUG))
#include "ShadersPrecompiled/GAFPrecompiledShaders.h"
        program = new cocos2d::GLProgram();
        program->autorelease();
        program->initWithPrecompiledProgramByteArray((const GLchar*)kGAFSpriteWithAlphaShaderProgramCache,
            sizeof(kGAFSpriteWithAlphaShaderProgramCache));
        program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        program->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        CCShaderCache::sharedShaderCache()->addProgram(program, kGAFSpriteWithAlphaShaderProgramCacheKey);
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
            CCLOGERROR("Cannot load program for GAFSpriteWithAlpha.");
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

uint32_t GAFSpriteWithAlpha::setUniforms()
{
#ifndef ENABLE_NEW_UNIFORM_SETTER
#define ENABLE_NEW_UNIFORM_SETTER 0
#endif
    struct Hash
    {
        int       program;
        uint32_t  texture;
        BlendFunc blend;
        decltype(m_colorTransformMult)      a;
        decltype(m_colorTransformOffsets)   b;
        decltype(m_colorTransformMult.w)    c;
        Mat4    d;
        Vec4    e;
        Mat4    f;
        Vec4    g;
    };

    GLProgramState* state = getGLProgramState();

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
        state->setUniformMat4("colorMatrix", Mat4(m_colorMatrixFilterData->matrix));
        state->setUniformVec4("colorMatrix2", Vec4(m_colorMatrixFilterData->matrix2));
#endif
        hash.f = Mat4(m_colorMatrixFilterData->matrix);
        hash.g = Vec4(m_colorMatrixFilterData->matrix2);
    }

    return XXH32((void*)&hash, sizeof(Hash), 0);
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * mults, const GLfloat * offsets)
{
    m_colorTransformMult = Vec4(mults);
    m_colorTransformOffsets = Vec4(offsets);
    _setBlendingFunc();
#if CHECK_CTX_IDENTITY
    setShaderProgram(programForShader());
#endif
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * colorTransform)
{
    m_colorTransformMult = Vec4(colorTransform);
    m_colorTransformOffsets = Vec4(&colorTransform[4]);

    _setBlendingFunc();
#if CHECK_CTX_IDENTITY
    setShaderProgram(programForShader());
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

bool GAFSpriteWithAlpha::isCTXIdentity() const
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

