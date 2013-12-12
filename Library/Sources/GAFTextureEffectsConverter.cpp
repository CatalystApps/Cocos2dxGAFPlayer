#include "GAFTextureEffectsConverter.h"
#include "misc_nodes/CCRenderTexture.h"
#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "cocoa/CCDictionary.h"
#include "GAFShaderManager.h"

GAFTextureEffectsConverter::GAFTextureEffectsConverter()
{
	_vertexShaderUniforms = new CCDictionary();
}

GAFTextureEffectsConverter::~GAFTextureEffectsConverter()
{
	CC_SAFE_RELEASE(_vertexShaderUniforms);
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

CCRenderTexture * GAFTextureEffectsConverter::gaussianBlurredTextureFromTexture(CCTexture2D * aTexture, const CCRect& rect, float aBlurRadiusX, float aBlurRadiusY)
{
	const int kGaussianKernelSize = 9;
    
    CCSize rTextureSize = CCSizeMake(rect.size.width + 2 * (kGaussianKernelSize / 2) * aBlurRadiusX,
                                     rect.size.height + 2 * (kGaussianKernelSize / 2) * aBlurRadiusY);
	
	CCRenderTexture *rTexture1 = CCRenderTexture::create(rTextureSize.width, rTextureSize.height);
	CCRenderTexture *rTexture2 = CCRenderTexture::create(rTextureSize.width, rTextureSize.height);
	CCGLProgram * shader = programForBlurShaderWithName("GaussianBlur", "Shaders/GaussianBlurVertexShader.vs", "Shaders/GaussianBlurFragmentShader.fs");
	if (!shader)
	{
		return NULL;
	}
	GLint texelWidthOffset = (GLint)glGetUniformLocation(shader->getProgram(), "texelWidthOffset");
    GLint texelHeightOffset = (GLint)glGetUniformLocation(shader->getProgram(), "texelHeightOffset");
	CHECK_GL_ERROR_DEBUG();	
	{
		CCSprite *sprite = CCSprite::createWithTexture(aTexture, rect);
        sprite->setPosition(CCPointMake(rTextureSize.width / 2, rTextureSize.height / 2));
		ccBlendFunc blend = { GL_ONE, GL_ZERO };
        sprite->setBlendFunc(blend);
        
        rTexture2->beginWithClear(0, 0, 0, 0);
		sprite->visit();
		rTexture2->end();
	}
	CHECK_GL_ERROR_DEBUG();
    {
        // Render rTexture2 to rTexture1 (horizontal)
        GLfloat texelWidthValue = aBlurRadiusX / (GLfloat)rTextureSize.width;
        GLfloat texelHeightValue = 0;
        
        rTexture2->getSprite()->setPosition(CCPointMake(rTextureSize.width / 2, rTextureSize.height / 2));
        rTexture2->getSprite()->setShaderProgram(shader);
		shader->use();
        glUniform1f(texelWidthOffset, texelWidthValue);
        glUniform1f(texelHeightOffset, texelHeightValue);        
		ccBlendFunc blend = { GL_ONE, GL_ZERO };
		rTexture2->getSprite()->setBlendFunc(blend);
        rTexture1->beginWithClear(0, 0, 0, 0);
		rTexture2->getSprite()->visit();
		rTexture1->end();
    }
	
	CHECK_GL_ERROR_DEBUG();    
    {
        // Render rTexture1 to rTexture2 (vertical)
        GLfloat texelWidthValue = 0;
        GLfloat texelHeightValue = aBlurRadiusY / (GLfloat)rTextureSize.height;
        
        rTexture1->getSprite()->setPosition(CCPointMake(rTextureSize.width / 2, rTextureSize.height / 2));
        rTexture1->getSprite()->setShaderProgram(shader);
		shader->use();
        glUniform1f(texelWidthOffset, texelWidthValue);
        glUniform1f(texelHeightOffset, texelHeightValue);
		ccBlendFunc blend = { GL_ONE, GL_ZERO };
		rTexture1->getSprite()->setBlendFunc(blend);
        rTexture2->beginWithClear(0, 0, 0, 0);
		rTexture1->getSprite()->visit();
		rTexture2->end();
    }    
    CHECK_GL_ERROR_DEBUG();    
    return rTexture2;
}

CCGLProgram * GAFTextureEffectsConverter::programForBlurShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile)
{
	CCGLProgram *program = CCShaderCache::sharedShaderCache()->programForKey(aShaderName);
	if (!program)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && !defined(_DEBUG))
		#include "ShadersPrecompiled/GAFPrecompiledShaders.h"

		if (std::string(aShaderName) != "GaussianBlur") //make only for GaussianBlur
		{
			CCAssert(false, "");
			return NULL;
		}

		program = new CCGLProgram();
		program->autorelease();
		program->initWithPrecompiledProgramByteArray((const GLchar*)GaussianBlur, sizeof(GaussianBlur));
		program->addAttribute("position", kCCVertexAttrib_Position);
		program->addAttribute("inputTextureCoordinate", kCCVertexAttrib_TexCoords);
		program->updateUniforms();
		CHECK_GL_ERROR_DEBUG();
		CCShaderCache::sharedShaderCache()->addProgram(program, aShaderName);
#else
		program = new CCGLProgram();
		program->initWithVertexShaderFilename(aVertexShaderFile, aFragmentShaderFile);
		if (program)
		{
			program->addAttribute("position", kCCVertexAttrib_Position);
			program->addAttribute("inputTextureCoordinate", kCCVertexAttrib_TexCoords);
			program->link();
			program->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
			CCShaderCache::sharedShaderCache()->addProgram(program, aShaderName);
		}
		else
		{
			CCLOGWARN("Cannot load program for %s.", aShaderName);
			return NULL;
		}
#endif
	}
	return program;
}

#if CC_ENABLE_CACHE_TEXTURE_DATA
void _GAFreloadBlurShader()
{
	CCGLProgram *program = CCShaderCache::sharedShaderCache()->programForKey("GaussianBlur");
	if (!program)
	{
		return;
	}
	program->reset();
	program->initWithVertexShaderFilename("Shaders/GaussianBlurVertexShader.vs", "Shaders/GaussianBlurFragmentShader.fs");
	if (program)
	{
		program->addAttribute("position", kCCVertexAttrib_Position);
		program->addAttribute("inputTextureCoordinate", kCCVertexAttrib_TexCoords);
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