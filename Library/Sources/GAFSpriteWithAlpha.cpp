#include "GAFPrecompiled.h"
#include "GAFSpriteWithAlpha.h"
#include "GAFShaderManager.h"
#include "GAFTextureEffectsConverter.h"

#include "GAFSubobjectState.h"

#include "GAFSpriteWithAlpha.h"
#include "GAFFilterData.h"

#include "GAFShaderManager.h"

using namespace cocos2d;

static int colorTransformLocation = -1;
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
    memset(m_colorMatrixIdentity1, 0, sizeof(float)* 16);

    m_colorMatrixIdentity1[0] = 1.f;
    m_colorMatrixIdentity1[5] = 1.f;
    m_colorMatrixIdentity1[10] = 1.f;
    m_colorMatrixIdentity1[15] = 1.f;

    memset(m_colorMatrixIdentity2, 0, sizeof(float)* 4);

}

GAFSpriteWithAlpha::~GAFSpriteWithAlpha()
{
    CC_SAFE_RELEASE(m_initialTexture);
#if CC_ENABLE_CACHE_TEXTURE_DATA
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
#endif
}

bool GAFSpriteWithAlpha::initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated)
{
    if (GAFSprite::initWithTexture(pTexture, rect, rotated))
    {
        m_initialTexture = pTexture;
        m_initialTexture->retain();
        m_initialTextureRect = rect;
        for (int i = 0; i < 4; ++i)
        {
            _colorTransform[i] = 1.0f;
            _colorTransform[i + 4] = 0;
        }
        _setBlendingFunc();
        setShaderProgram(programForShader());
        return true;
    }
    else
    {
        return false;
    }
}

void GAFSpriteWithAlpha::reset()
{
    CCGLProgram * program = CCShaderCache::sharedShaderCache()->programForKey(kGAFSpriteWithAlphaShaderProgramCacheKey);

    bool isCTXidt = false;

    if (!program)
    {
#if CHECK_CTX_IDENTITY
        program = CCShaderCache::sharedShaderCache()->programForKey(kGAFSpriteWithAlphaShaderProgramCache_noCTX);

        if (!program)
        {
            return;
        }

        isCTXidt = true;
#else
        return;
#endif
    }

    program->reset();

    if (isCTXidt)
    {
        program = GAFShaderManager::getInstance()->createWithFragmentFilename(ccPositionTextureColor_vert, kAlphaFragmentShaderFilename_noCTX);
    }
    else
    {
        program = GAFShaderManager::getInstance()->createWithFragmentFilename(ccPositionTextureColor_vert, kAlphaFragmentShaderFilename);
    }

    if (program)
    {
        program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        program->link();
        CHECK_GL_ERROR_DEBUG();

        program->updateUniforms();

        CHECK_GL_ERROR_DEBUG();

        program->use();

        CCLOG("Restoring shader1");

        colorTransformLocation = glGetUniformLocation(program->getProgram(), "colorTransform");
        colorMatrixLocation = glGetUniformLocation(program->getProgram(), "colorMatrix");
        colorMatrixLocation2 = glGetUniformLocation(program->getProgram(), "colorMatrix2");
        fragmentAlphaLocation = glGetUniformLocation(program->getProgram(), "fragmentAlpha");

        CCShaderCache::sharedShaderCache()->addProgram(program, isCTXidt ? kGAFSpriteWithAlphaShaderProgramCache_noCTX : kGAFSpriteWithAlphaShaderProgramCacheKey);

        CHECK_GL_ERROR_DEBUG();
    }
    else
    {

        CCAssert(false, "Can not RELOAD GAFSpriteWithAlpha");
    }
}

CCGLProgram * GAFSpriteWithAlpha::programForShader()
{
#if CHECK_CTX_IDENTITY
    const bool isCTXidt = isCTXIdentity();
#else
    const bool isCTXidt = false;
#endif

    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(isCTXidt ? kGAFSpriteWithAlphaShaderProgramCache_noCTX : kGAFSpriteWithAlphaShaderProgramCacheKey);

    if (!program)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && !defined(_DEBUG))
#include "ShadersPrecompiled/GAFPrecompiledShaders.h"
        program = new CCGLProgram();
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
        if (isCTXidt)
        {
            program = GAFShaderManager::getInstance()->createWithFragmentFilename(ccPositionTextureColor_vert, kAlphaFragmentShaderFilename_noCTX);
        }
        else
        {
            program = GAFShaderManager::getInstance()->createWithFragmentFilename(ccPositionTextureColor_vert, kAlphaFragmentShaderFilename);
        }

        if (program)
        {
            program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
            program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
            program->link();
            program->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
            CCShaderCache::sharedShaderCache()->addProgram(program, isCTXidt ? kGAFSpriteWithAlphaShaderProgramCache_noCTX : kGAFSpriteWithAlphaShaderProgramCacheKey);
        }
        else
        {
            CCLOGERROR("Cannot load program for GAFSpriteWithAlpha.");
            CC_SAFE_DELETE(program);
            return NULL;
        }

        CHECK_GL_ERROR_DEBUG();
#endif
    }
    program->use();

    colorTransformLocation = glGetUniformLocation(program->getProgram(), "colorTransform");
    colorMatrixLocation = glGetUniformLocation(program->getProgram(), "colorMatrix");
    colorMatrixLocation2 = glGetUniformLocation(program->getProgram(), "colorMatrix2");
    fragmentAlphaLocation = glGetUniformLocation(program->getProgram(), "fragmentAlpha");

    

    return program;
}

void GAFSpriteWithAlpha::updateTextureWithEffects()
{
    if (!m_blurFilterData && !m_glowFilterData)
    {
        setTexture(m_initialTexture);
        setTextureRect(m_initialTextureRect, false, m_initialTextureRect.size);
        setFlipY(false);
    }
    else
    {
        GAFTextureEffectsConverter * converter = GAFTextureEffectsConverter::sharedConverter();

        CCRenderTexture * resultTex = NULL;

        if (m_blurFilterData)
        {
            resultTex = converter->gaussianBlurredTextureFromTexture(m_initialTexture, m_initialTextureRect, m_blurFilterData->blurSize.width, 
                m_blurFilterData->blurSize.height);
        }
        else if (m_glowFilterData)
        {
            resultTex = converter->glowTextureFromTexture(m_initialTexture, m_initialTextureRect, m_glowFilterData);
        }

        if (resultTex)
        {
            setTexture(resultTex->getSprite()->getTexture());
            setFlipY(true);
            CCRect texureRect = CCRectMake(0, 0, resultTex->getSprite()->getContentSize().width, resultTex->getSprite()->getContentSize().height);
            setTextureRect(texureRect, false, texureRect.size);
        }
    }
}

void GAFSpriteWithAlpha::setUniformsForFragmentShader()
{
    setShaderProgram(programForShader());

    if (colorTransformLocation > -1)
    {
        glUniform4fv(colorTransformLocation, 2, _colorTransform);
        CHECK_GL_ERROR_DEBUG();
    }
    else if (fragmentAlphaLocation > -1)
    {
        glUniform1f(fragmentAlphaLocation, _colorTransform[GAFCTI_A]);
        CHECK_GL_ERROR_DEBUG();
    }


    if (colorMatrixLocation > -1 && colorMatrixLocation2 > -1)
    {
        if (!m_colorMatrixFilterData)
        {
            glUniformMatrix4fv(colorMatrixLocation, 1, false, m_colorMatrixIdentity1);
            glUniform4fv(colorMatrixLocation2, 1, m_colorMatrixIdentity2);
            CHECK_GL_ERROR_DEBUG();
        }
        else
        {
            glUniformMatrix4fv(colorMatrixLocation, 1, false, m_colorMatrixFilterData->matrix);
            glUniform4fv(colorMatrixLocation2, 1, m_colorMatrixFilterData->matrix2);
            CHECK_GL_ERROR_DEBUG();
        }
    }
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * mults, const GLfloat * offsets)
{
    for (int i = 0; i < 4; ++i)
    {
        _colorTransform[i] = mults[i];
        _colorTransform[i + 4] = offsets[i];
    }
    _setBlendingFunc();
#if CHECK_CTX_IDENTITY
    setShaderProgram(programForShader());
#endif
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * colorTransform)
{
    for (int i = 0; i < 8; ++i)
    {
        _colorTransform[i] = colorTransform[i];
    }
    _setBlendingFunc();
#if CHECK_CTX_IDENTITY
    setShaderProgram(programForShader());
#endif
}

void GAFSpriteWithAlpha::_setBlendingFunc()
{
    ccBlendFunc bf;
    bf.src = GL_ONE;
    bf.dst = GL_ONE_MINUS_SRC_ALPHA;
    setBlendFunc(bf);
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

const GLfloat * GAFSpriteWithAlpha::getColorTransform() const
{
    return _colorTransform;
}

CCTexture2D* GAFSpriteWithAlpha::getInitialTexture() const
{
    return m_initialTexture;
}

const CCRect& GAFSpriteWithAlpha::getInitialTextureRect() const
{
    return m_initialTextureRect;
}

bool GAFSpriteWithAlpha::isCTXIdentity() const
{
    for (int i = 0; i < 4; ++i)
    {
        if (_colorTransform[i] != 1.0f)
        {
            return false;
        }

        if (_colorTransform[i + 4] != 0)
        {
            return false;
        }
    }

    return true;
}

