#include "GAFPrecompiled.h"
#include "GAFTextureEffectsConverter.h"
#include "GAFShaderManager.h"

#include "GAFFilterData.h"

const int kGaussianKernelSize = 9;

GAFTextureEffectsConverter::GAFTextureEffectsConverter()
{

}

GAFTextureEffectsConverter::~GAFTextureEffectsConverter()
{

}

static GAFTextureEffectsConverter * _sharedConverter = NULL;

GAFTextureEffectsConverter * GAFTextureEffectsConverter::sharedConverter()
{
    if (!_sharedConverter)
    {
        _sharedConverter = new GAFTextureEffectsConverter();
    }
    return _sharedConverter;
}

cocos2d::RenderTexture* GAFTextureEffectsConverter::dropShadowTextureFromTexture(cocos2d::Texture2D * aTexture, const cocos2d::Rect& rect, GAFDropShadowFilterData* data)
{
    const float blurRadiusX = (data->blurSize.width / 4.f);
    const float blurRadiusY = (data->blurSize.height / 4.f);

    cocos2d::Size rTextureSize = cocos2d::Size(rect.size.width + 2 * (kGaussianKernelSize / 2) * blurRadiusX,
        rect.size.height + 2 * (kGaussianKernelSize / 2) * blurRadiusY);

    cocos2d::GLProgram * shader = programForBlurShaderWithName("GaussianBlur", "Shaders/GlowVertexShader.vs", "Shaders/GlowFragmentShader.fs");

    cocos2d::RenderTexture *rTexture1 = cocos2d::RenderTexture::create(
        static_cast<int>(rTextureSize.width),
        static_cast<int>(rTextureSize.height));

    cocos2d::RenderTexture *rTexture2 = cocos2d::RenderTexture::create(
        static_cast<int>(rTextureSize.width),
        static_cast<int>(rTextureSize.height));

    if (!shader)
    {
        return NULL;
    }

    GLint texelWidthOffset = glGetUniformLocation(shader->getProgram(), "texelWidthOffset");
    GLint texelHeightOffset = glGetUniformLocation(shader->getProgram(), "texelHeightOffset");
    GLint glowColor = glGetUniformLocation(shader->getProgram(), "glowColor");

    CHECK_GL_ERROR_DEBUG();
    {
        cocos2d::Sprite *sprite = cocos2d::Sprite::createWithTexture(aTexture, rect);
        sprite->setPosition(cocos2d::Vect(rTextureSize.width / 2, rTextureSize.height / 2));
        cocos2d::BlendFunc blend = { GL_ONE, GL_ZERO };
        sprite->setBlendFunc(blend);

        rTexture2->beginWithClear(0, 0, 0, 0);
        sprite->visit();
        rTexture2->end();
    }
    CHECK_GL_ERROR_DEBUG();
    {
        // Render rTexture2 to rTexture1 (horizontal)
        GLfloat texelWidthValue = blurRadiusX / (GLfloat)rTextureSize.width;
        GLfloat texelHeightValue = 0;

        rTexture2->getSprite()->setPosition(cocos2d::Vect(rTextureSize.width / 2, rTextureSize.height / 2));
        rTexture2->getSprite()->setGLProgram(shader);
        shader->use();

        glUniform1f(texelWidthOffset, texelWidthValue);
        glUniform1f(texelHeightOffset, texelHeightValue);

        glUniform4fv(glowColor, 1, &data->color.r);

        cocos2d::BlendFunc blend = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        rTexture2->getSprite()->setBlendFunc(blend);
        rTexture1->beginWithClear(0, 0, 0, 0);
        rTexture2->getSprite()->visit();
        rTexture1->end();
    }

    CHECK_GL_ERROR_DEBUG();
    {
        // Render rTexture1 to rTexture2 (vertical)
        GLfloat texelWidthValue = 0;
        GLfloat texelHeightValue = blurRadiusY / (GLfloat)rTextureSize.height;

        rTexture1->getSprite()->setPosition(cocos2d::Vect(rTextureSize.width / 2, rTextureSize.height / 2));
        rTexture1->getSprite()->setGLProgram(shader);
        shader->use();

        glUniform1f(texelWidthOffset, texelWidthValue);
        glUniform1f(texelHeightOffset, texelHeightValue);
        glUniform4fv(glowColor, 1, &data->color.r);

        cocos2d::BlendFunc blend = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        rTexture1->getSprite()->setBlendFunc(blend);
        rTexture2->beginWithClear(0, 0, 0, 0);
        rTexture1->getSprite()->visit();
        rTexture2->end();
    }

    return rTexture2;
}

cocos2d::RenderTexture * GAFTextureEffectsConverter::glowTextureFromTexture(cocos2d::Texture2D * aTexture, const cocos2d::Rect& rect, GAFGlowFilterData* data)
{
    const float blurRadiusX = (data->blurSize.width / 4.f);
    const float blurRadiusY = (data->blurSize.height / 4.f);

    cocos2d::Size rTextureSize = cocos2d::Size(rect.size.width + 2 * (kGaussianKernelSize / 2) * blurRadiusX,
        rect.size.height + 2 * (kGaussianKernelSize / 2) * blurRadiusY);

    cocos2d::RenderTexture *rTexture1 = cocos2d::RenderTexture::create(static_cast<int>(rTextureSize.width),
        static_cast<int>(rTextureSize.height));

    cocos2d::RenderTexture *rTexture2 = cocos2d::RenderTexture::create(static_cast<int>(rTextureSize.width),
        static_cast<int>(rTextureSize.height));

    cocos2d::GLProgram * shader = programForBlurShaderWithName("GaussianBlur", "Shaders/GlowVertexShader.vs", "Shaders/GlowFragmentShader.fs");

    if (!shader)
    {
        return NULL;
    }

    GLint texelWidthOffset = glGetUniformLocation(shader->getProgram(), "texelWidthOffset");
    GLint texelHeightOffset = glGetUniformLocation(shader->getProgram(), "texelHeightOffset");
    GLint glowColor = glGetUniformLocation(shader->getProgram(), "glowColor");
    
    CHECK_GL_ERROR_DEBUG();
    {
        cocos2d::Sprite *sprite = cocos2d::Sprite::createWithTexture(aTexture, rect);
        sprite->setPosition(cocos2d::Vect(rTextureSize.width / 2, rTextureSize.height / 2));
        cocos2d::BlendFunc blend = { GL_ONE, GL_ZERO };
        sprite->setBlendFunc(blend);

        rTexture2->beginWithClear(0, 0, 0, 0);
        sprite->visit();
        rTexture2->end();
    }
    CHECK_GL_ERROR_DEBUG();
    {
        // Render rTexture2 to rTexture1 (horizontal)
        GLfloat texelWidthValue = blurRadiusX / (GLfloat)rTextureSize.width;
        GLfloat texelHeightValue = 0;

        rTexture2->getSprite()->setPosition(cocos2d::Vect(rTextureSize.width / 2, rTextureSize.height / 2));
        rTexture2->getSprite()->setGLProgram(shader);
        shader->use();

        glUniform1f(texelWidthOffset, texelWidthValue);
        glUniform1f(texelHeightOffset, texelHeightValue);

        glUniform4fv(glowColor, 1, &data->color.r);

        cocos2d::BlendFunc blend = { GL_ONE, GL_ZERO };
        rTexture2->getSprite()->setBlendFunc(blend);
        rTexture1->beginWithClear(0, 0, 0, 0);
        rTexture2->getSprite()->visit();
        rTexture1->end();
    }

    CHECK_GL_ERROR_DEBUG();
    {
        // Render rTexture1 to rTexture2 (vertical)
        GLfloat texelWidthValue = 0;
        GLfloat texelHeightValue = blurRadiusY / (GLfloat)rTextureSize.height;

        rTexture1->getSprite()->setPosition(cocos2d::Vect(rTextureSize.width / 2, rTextureSize.height / 2));
        rTexture1->getSprite()->setGLProgram(shader);
        shader->use();

        glUniform1f(texelWidthOffset, texelWidthValue);
        glUniform1f(texelHeightOffset, texelHeightValue);
        glUniform4fv(glowColor, 1, &data->color.r);

        cocos2d::BlendFunc blend = { GL_ONE, GL_ZERO };
        rTexture1->getSprite()->setBlendFunc(blend);
        rTexture2->beginWithClear(0, 0, 0, 0);
        rTexture1->getSprite()->visit();
        rTexture2->end();
    }
    CHECK_GL_ERROR_DEBUG();
    {
        cocos2d::Sprite *sprite = cocos2d::Sprite::createWithTexture(aTexture, rect);
        sprite->setPosition(cocos2d::Vect(rTextureSize.width / 2, rTextureSize.height / 2));

        rTexture2->begin();
        sprite->visit();
        rTexture2->end();
    }
    CHECK_GL_ERROR_DEBUG();

    return rTexture2;
}

cocos2d::RenderTexture * GAFTextureEffectsConverter::gaussianBlurredTextureFromTexture(cocos2d::Texture2D * aTexture, const cocos2d::Rect& rect, float aBlurRadiusX, float aBlurRadiusY)
{
    const float blurRadiusX = (aBlurRadiusX / 4.f);
    const float blurRadiusY = (aBlurRadiusY / 4.f);

    cocos2d::Size rTextureSize = cocos2d::Size(rect.size.width + 2 * (kGaussianKernelSize / 2) * blurRadiusX,
        rect.size.height + 2 * (kGaussianKernelSize / 2) * blurRadiusY);

    const int rTextureW = static_cast<int>(rTextureSize.width);
    const int rTextureH = static_cast<int>(rTextureSize.height);

    //cocos2d::RenderTexture *rTexture1 = cocos2d::RenderTexture::create(rTextureW, rTextureH, cocos2d::Texture2D::PixelFormat::RGBA8888);

    cocos2d::RenderTexture *rTexture2 = cocos2d::RenderTexture::create(rect.size.width, rect.size.height, cocos2d::Texture2D::PixelFormat::RGBA8888);

    cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
    cocos2d::Size pixelSize = cocos2d::Director::getInstance()->getWinSizeInPixels();
    cocos2d::Vect center = cocos2d::Vect(winSize.width / 2, winSize.height / 2);

    cocos2d::Mat4 mat;

    /*cocos2d::GLProgram * shader = programForBlurShaderWithName("GaussianBlur", "Shaders/GaussianBlurVertexShader.vs", "Shaders/GaussianBlurFragmentShader.fs");
    if (!shader)
    {
        return NULL;
    }

    GLint texelWidthOffset = (GLint)glGetUniformLocation(shader->getProgram(), "texelWidthOffset");
    GLint texelHeightOffset = (GLint)glGetUniformLocation(shader->getProgram(), "texelHeightOffset");*/
    CHECK_GL_ERROR_DEBUG();
    {
        cocos2d::Sprite *sprite = cocos2d::Sprite::createWithTexture(aTexture, rect);
        sprite->setPosition(center);
        rTexture2->setPosition(center);

        rTexture2->setVirtualViewport(cocos2d::Vect(winSize.width / 2 - rect.size.width / 2, winSize.height / 2 - rect.size.height / 2),
            cocos2d::Rect(0, 0, winSize.width, winSize.height), cocos2d::Rect(0, 0, pixelSize.width, pixelSize.height));

        //sprite->setBlendFunc(cocos2d::BlendFunc::DISABLE);

        rTexture2->begin();

        sprite->visit(cocos2d::Director::getInstance()->getRenderer(), mat, true);
        rTexture2->end();
    }
    /*CHECK_GL_ERROR_DEBUG();
    {
        // Render rTexture2 to rTexture1 (horizontal)
        GLfloat texelWidthValue = blurRadiusX / (GLfloat)rTextureSize.width;
        GLfloat texelHeightValue = 0;

        rTexture2->getSprite()->setPosition(center);
        rTexture2->getSprite()->setShaderProgram(shader);
        shader->use();
        glUniform1f(texelWidthOffset, texelWidthValue);
        glUniform1f(texelHeightOffset, texelHeightValue);

        rTexture2->getSprite()->setBlendFunc(cocos2d::BlendFunc::DISABLE);

        rTexture1->beginWithClear(0, 0, 0, 0);
        rTexture2->getSprite()->visit(cocos2d::Director::getInstance()->getRenderer(), mat, true););
        rTexture1->end();
    }*/

    CHECK_GL_ERROR_DEBUG();
    /*{
        // Render rTexture1 to rTexture2 (vertical)
        GLfloat texelWidthValue = 0;
        GLfloat texelHeightValue = blurRadiusY / (GLfloat)rTextureSize.height;

        rTexture1->getSprite()->setPosition(cocos2d::Vect(rTextureSize.width / 2, rTextureSize.height / 2));
        rTexture1->getSprite()->setShaderProgram(shader);
        shader->use();
        glUniform1f(texelWidthOffset, texelWidthValue);
        glUniform1f(texelHeightOffset, texelHeightValue);

        rTexture1->getSprite()->setBlendFunc(cocos2d::BlendFunc::DISABLE);

        rTexture2->beginWithClear(0, 0, 0, 0);
        rTexture1->getSprite()->visit();
        rTexture2->end();
    }
    CHECK_GL_ERROR_DEBUG();*/

    return rTexture2;
}

cocos2d::GLProgram * GAFTextureEffectsConverter::programForGlowShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile)
{
    cocos2d::GLProgram *program = cocos2d::ShaderCache::getInstance()->getGLProgram(aShaderName);

    if (!program)
    {
        program = new cocos2d::GLProgram();
        bool ok = program->initWithFilenames(aVertexShaderFile, aFragmentShaderFile);

        if (ok)
        {
            program->bindAttribLocation("position", cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
            program->bindAttribLocation("inputTextureCoordinate", cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
            program->link();
            program->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
            cocos2d::ShaderCache::getInstance()->addGLProgram(program, aShaderName);
        }
        else
        {
            CCLOG("Cannot load program for %s.", aShaderName);
            return NULL;
        }
    }

    return program;
}

cocos2d::GLProgram * GAFTextureEffectsConverter::programForBlurShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile)
{
    cocos2d::GLProgram *program = cocos2d::ShaderCache::getInstance()->getGLProgram(aShaderName);
    if (!program)
    {
#if 0  //TODO //CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && !defined(_DEBUG))
#include "ShadersPrecompiled/GAFPrecompiledShaders.h"

        if (std::string(aShaderName) != "GaussianBlur") //make only for GaussianBlur
        {
            CCAssert(false, "");
            return NULL;
        }

        program = new cocos2d::GLProgram();
        program->autorelease();
        program->initWithPrecompiledProgramByteArray((const GLchar*)GaussianBlur, sizeof(GaussianBlur));
        program->addAttribute("position", kCCVertexAttrib_Position);
        program->addAttribute("inputTextureCoordinate", kCCVertexAttrib_TexCoords);
        program->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        CCShaderCache::sharedShaderCache()->addProgram(program, aShaderName);
#else
        program = new cocos2d::GLProgram();
        program->initWithFilenames(aVertexShaderFile, aFragmentShaderFile);
        if (program)
        {
            program->bindAttribLocation("position", cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
            program->bindAttribLocation("inputTextureCoordinate", cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
            program->link();
            program->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
            cocos2d::ShaderCache::getInstance()->addGLProgram(program, aShaderName);
        }
        else
        {
            CCLOG("Cannot load program for %s.", aShaderName);
            return NULL;
        }
#endif
    }
    return program;
}

#if CC_ENABLE_CACHE_TEXTURE_DATA
void _GAFreloadBlurShader()
{
    cocos2d::GLProgram *program = cocos2d::ShaderCache::getInstance()->getProgram("GaussianBlur");
    if (!program)
    {
        return;
    }
    program->reset();
    program->initWithVertexShaderFilename("Shaders/GaussianBlurVertexShader.vs", "Shaders/GaussianBlurFragmentShader.fs");
    if (program)
    {
        program->addAttribute("position", cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
        program->addAttribute("inputTextureCoordinate", cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD);
        program->link();
        program->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
    }
    else
    {
        CCAssert(false, "Can not RELOAD GAFTextureEffectsConverter");
    }
    CCLOGERROR("GAFTextureEffectsConverter RELOADED");
}
#endif
