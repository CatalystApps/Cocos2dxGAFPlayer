#include "GAFPrecompiled.h"
#include "GAFSpriteWithAlpha.h"
#include "GAFShaderManager.h"
#include "GAFTextureEffectsConverter.h"

#include "GAFSubobjectState.h"

#include "GAFSpriteWithAlpha.h"
#include "GAFFilterData.h"

#define CHECK_CTX_IDENTITY 0

static int colorTransformLocation = -1;
static int fragmentAlphaLocation = -1;
static int colorMatrixLocation   = -1;
static int colorMatrixLocation2  = -1;

static const char * kAlphaFragmentShaderFilename = "Shaders/pcShader_PositionTextureAlpha_frag.fs";
static const char * kAlphaFragmentShaderFilename_noCTX = "Shaders/pcShader_PositionTextureAlpha_frag_noCTX.fs";

static const char * kGAFSpriteWithAlphaShaderProgramCache_noCTX = "kGAFSpriteWithAlphaShaderProgramCache_noCTX";
static const char * kGAFSpriteWithAlphaShaderProgramCacheKey = "kGAFSpriteWithAlphaShaderProgramCache";

GAFSpriteWithAlpha::GAFSpriteWithAlpha()
:
m_initialTexture(NULL),
m_colorMatrixFilterData(NULL),
m_glowFilterData(NULL),
m_blurFilterData(NULL),
m_filter(NULL)
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
}

bool GAFSpriteWithAlpha::initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated)
{
    if (GAFSprite::initWithTexture(pTexture, rect, rotated))
    {
        m_initialTexture = pTexture;
        m_initialTexture->retain();
        m_initialTextureRect = rect;
        for (int i = 0; i < 4; ++i)
        {
            m_colorTransform[i] = 1.0f;
            m_colorTransform[i + 4] = 0;
        }
        _setBlendingFunc();
        setGLProgram(programForShader());
        return true;
    }
    else
    {
        return false;
    }
}

cocos2d::GLProgram * GAFSpriteWithAlpha::programForShader()
{
#if CHECK_CTX_IDENTITY
    const bool isCTXidt = isCTXIdentity();
#else
    const bool isCTXidt = false;
#endif

    cocos2d::GLProgram* program = cocos2d::ShaderCache::getInstance()->getGLProgram(isCTXidt ? kGAFSpriteWithAlphaShaderProgramCache_noCTX : kGAFSpriteWithAlphaShaderProgramCacheKey);

    if (!program)
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
        if (isCTXidt)
        {
            program = GAFShaderManager::createWithFragmentFilename(cocos2d::ccPositionTextureColor_vert, kAlphaFragmentShaderFilename_noCTX);
        }
        else
        {
            program = GAFShaderManager::createWithFragmentFilename(cocos2d::ccPositionTextureColor_vert, kAlphaFragmentShaderFilename);
        }

        if (program)
        {
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
            program->link();
            program->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
            cocos2d::ShaderCache::getInstance()->addGLProgram(program, isCTXidt ? kGAFSpriteWithAlphaShaderProgramCache_noCTX : kGAFSpriteWithAlphaShaderProgramCacheKey);
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

cocos2d::RenderTexture* GAFSpriteWithAlpha::_test()
{
    //m_filter = cocos2d::RenderTexture::create(m_initialTextureRect.size.width, m_initialTextureRect.size.height);

    auto s = cocos2d::Director::getInstance()->getWinSize();
    cocos2d::Vect center = cocos2d::Vect(s.width / 2, s.height / 2);

    auto spr_nonpremulti = cocos2d::Sprite::createWithTexture(m_initialTexture, m_initialTextureRect);

    spr_nonpremulti->setPosition(center);
    m_filter->setPosition(center);

    //m_filter->setKeepMatrix(true);
    cocos2d::Size pixelSize = cocos2d::Director::getInstance()->getWinSizeInPixels();

    m_filter->setVirtualViewport(cocos2d::Vect(s.width / 2 - 184 / 2, s.height / 2 - 203 / 2),
        cocos2d::Rect(0, 0, s.width, s.height), cocos2d::Rect(0, 0, pixelSize.width, pixelSize.height));

    m_filter->begin();
    cocos2d::Mat4 mat;
    spr_nonpremulti->visit(cocos2d::Director::getInstance()->getRenderer(), mat, true);
    m_filter->end();

    setTexture(m_filter->getSprite()->getTexture());
    setFlippedY(true);
    cocos2d::Rect texureRect = cocos2d::Rect(0, 0, m_filter->getSprite()->getContentSize().width, m_filter->getSprite()->getContentSize().height);
    setTextureRect(texureRect, false, texureRect.size);

    return m_filter;
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
        GAFTextureEffectsConverter * converter = GAFTextureEffectsConverter::sharedConverter();

        cocos2d::RenderTexture * resultTex = NULL;

        if (m_blurFilterData)
        {
#if 0
            resultTex = converter->gaussianBlurredTextureFromTexture(m_initialTexture, m_initialTextureRect, m_blurFilterData->blurSize.width, m_blurFilterData->blurSize.height);

#else
            if (!m_filter)
            {
                m_filter = cocos2d::RenderTexture::create(m_initialTextureRect.size.width, m_initialTextureRect.size.height); 
                m_filter->retain();
            }

            cocos2d::RenderTexture* rt = _test();
            setTexture(rt->getSprite()->getTexture());
            setFlippedY(true);
            cocos2d::Rect texureRect = cocos2d::Rect(0, 0, m_filter->getSprite()->getContentSize().width, m_filter->getSprite()->getContentSize().height);
            setTextureRect(texureRect, false, texureRect.size);
#endif
           /* m_filter->begin();
            cocos2d::Sprite* spr = cocos2d::Sprite::createWithTexture(m_initialTexture, m_initialTextureRect);
            spr->setPosition(cocos2d::Vect(m_initialTextureRect.size.width / 2, m_initialTextureRect.size.height / 2));
            spr->visit();
            m_filter->end();

            setTexture(m_filter->getSprite()->getTexture());
            setFlippedY(true);
            cocos2d::Rect texureRect = cocos2d::Rect(0, 0, m_filter->getSprite()->getContentSize().width, m_filter->getSprite()->getContentSize().height);
            setTextureRect(texureRect, false, texureRect.size);*/

            /*resultTex = cocos2d::RenderTexture::create(m_initialTextureRect.size.width , m_initialTextureRect.size.height);

            cocos2d::Sprite* spr = cocos2d::Sprite::createWithTexture(m_initialTexture, m_initialTextureRect);

            spr->setPosition(cocos2d::Vect(0, 0));

            resultTex->beginWithClear(0, 1, 0, 1);
            spr->visit();
            resultTex->end();*/
        }
        else if (m_glowFilterData)
        {
            //resultTex = converter->glowTextureFromTexture(m_initialTexture, m_initialTextureRect, m_glowFilterData);
        }

        if (resultTex)
        {
            setTexture(resultTex->getSprite()->getTexture());
            setFlippedY(true);
            cocos2d::Rect texureRect = cocos2d::Rect(0, 0, resultTex->getSprite()->getContentSize().width, resultTex->getSprite()->getContentSize().height);
            setTextureRect(texureRect, false, texureRect.size);
        }
    }
}

void GAFSpriteWithAlpha::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
{
    

    GAFSprite::draw(renderer, transform, transformUpdated);
}

void GAFSpriteWithAlpha::setUniformsForFragmentShader()
{
    setGLProgram(programForShader());

    if (colorTransformLocation > -1)
    {
        glUniform4fv(colorTransformLocation, 2, m_colorTransform);
    }
    else if (fragmentAlphaLocation > -1)
    {
        glUniform1f(fragmentAlphaLocation, m_colorTransform[GAFCTI_A]);
    }


    if (colorMatrixLocation > -1 && colorMatrixLocation2 > -1)
    {
        if (!m_colorMatrixFilterData)
        {
            glUniformMatrix4fv(colorMatrixLocation, 1, false, m_colorMatrixIdentity1);
            glUniform4fv(colorMatrixLocation2, 1, m_colorMatrixIdentity2);
        }
        else
        {
            glUniformMatrix4fv(colorMatrixLocation, 1, false, m_colorMatrixFilterData->matrix);
            glUniform4fv(colorMatrixLocation2, 1, m_colorMatrixFilterData->matrix2);
        }
    }
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * mults, const GLfloat * offsets)
{
    for (int i = 0; i < 4; ++i)
    {
        m_colorTransform[i] = mults[i];
        m_colorTransform[i + 4] = offsets[i];
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
        m_colorTransform[i] = colorTransform[i];
    }
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

const GLfloat * GAFSpriteWithAlpha::getColorTransform() const
{
    return m_colorTransform;
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
    for (int i = 0; i < 4; ++i)
    {
        if (m_colorTransform[i] != 1.0f)
        {
            return false;
        }

        if (m_colorTransform[i + 4] != 0)
        {
            return false;
        }
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

