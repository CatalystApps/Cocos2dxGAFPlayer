#include "GAFSpriteWithAlpha.h"
#include "GAFShaderManager.h"
#include "GAFTextureEffectsConverter.h"
#include "misc_nodes/CCRenderTexture.h"

#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccShaders.h"

#include "GAFSpriteWithAlpha.h"

static const char * kAlphaFragmentShaderFilename = "Shaders/pcShader_PositionTextureAlpha_frag.fs";
static GLuint _colorTrasformLocation = 0;

GAFSpriteWithAlpha::GAFSpriteWithAlpha()
:
_initialTexture(NULL)
{
}

GAFSpriteWithAlpha::~GAFSpriteWithAlpha()
{
	CC_SAFE_RELEASE(_initialTexture);
}


bool GAFSpriteWithAlpha::initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated)
{
	if (GAFSprite::initWithTexture(pTexture, rect, rotated))
	{
		
		_initialTexture = pTexture;
		_initialTexture->retain();
		_initialTextureRect = rect;
		_blurRadius = CCSizeZero;
		for (int i = 0; i < 4; ++i)
		{
			_colorTransform[i]     = 1.0f;
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

CCGLProgram * GAFSpriteWithAlpha::programForShader()
{
	CCGLProgram * program = CCShaderCache::sharedShaderCache()->programForKey(kGAFSpriteWithAlphaShaderProgramCacheKey);
	if (!program)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && !defined(_DEBUG))
		#include "ShadersPrecompiled/GAFPrecompiledShaders.h"
		program = new CCGLProgram();
		program->autorelease();
		program->initWithPrecompiledProgramByteArray((const GLchar*)kGAFSpriteWithAlphaShaderProgramCache, 
			sizeof(kGAFSpriteWithAlphaShaderProgramCache));
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor,    kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->updateUniforms();
		CHECK_GL_ERROR_DEBUG();
		CCShaderCache::sharedShaderCache()->addProgram(program, kGAFSpriteWithAlphaShaderProgramCacheKey);
#else
		program = GAFShaderManager::createWithFragmentFilename(ccPositionTextureColor_vert, kAlphaFragmentShaderFilename);
		if (program)
		{
			program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			program->addAttribute(kCCAttributeNameColor,    kCCVertexAttrib_Color);
			program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			program->link();
			program->updateUniforms();
			CHECK_GL_ERROR_DEBUG();
			CCShaderCache::sharedShaderCache()->addProgram(program, kGAFSpriteWithAlphaShaderProgramCacheKey);
		}
		else
		{
			CCLOGERROR("Cannot load program for GAFSpriteWithAlpha.");
			CC_SAFE_DELETE(program);
			return NULL;
		}
#endif
	}
	program->use();
	_colorTrasformLocation = (GLuint)glGetUniformLocation(program->getProgram(), "colorTransform");
	if (_colorTrasformLocation <= 0)
	{
		CCLOGERROR("Cannot load program for GAFSpriteWithAlpha.");
	}
	return program;
}

void GAFSpriteWithAlpha::setBlurRadius(const CCSize& blurRadius)
{
	if (_blurRadius.width != blurRadius.width || _blurRadius.height != blurRadius.height)
	{
		_blurRadius = blurRadius;
		updateTextureWithEffects();
	}
}

void GAFSpriteWithAlpha::updateTextureWithEffects()
{
	if (0 == _blurRadius.width && 0 == _blurRadius.height)
	{
		setTexture(_initialTexture);
		setTextureRect(_initialTextureRect, false, _initialTextureRect.size);
		setFlipY(false);
	}
	else
	{
		GAFTextureEffectsConverter * converter = GAFTextureEffectsConverter::sharedConverter();
		CCRenderTexture * resultTex = converter->gaussianBlurredTextureFromTexture(_initialTexture, _initialTextureRect, _blurRadius.width, _blurRadius.height);
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
	glUniform4fv(_colorTrasformLocation, 2, _colorTransform);
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * mults, const GLfloat * offsets)
{
	for (int i = 0; i < 4; ++i)
	{
		_colorTransform[i]     = mults[i];
		_colorTransform[i + 4] = offsets[i];
	}
	_setBlendingFunc();
}

void GAFSpriteWithAlpha::setColorTransform(const GLfloat * colorTransform)
{
	for (int i = 0; i < 8; ++i)
	{
		_colorTransform[i] = colorTransform[i];
	}
	_setBlendingFunc();
}

void GAFSpriteWithAlpha::_setBlendingFunc()
{
	ccBlendFunc bf;
	bf.src = GL_ONE;
	bf.dst = GL_ONE_MINUS_SRC_ALPHA;
	setBlendFunc(bf);
}

#if CC_ENABLE_CACHE_TEXTURE_DATA
void _GAFreloadAlphaShader()
{
	CCGLProgram * program = CCShaderCache::sharedShaderCache()->programForKey(kGAFSpriteWithAlphaShaderProgramCacheKey);
	
	if (!program)
	{		
		return;
	}
	program->reset();
	program = GAFShaderManager::createWithFragmentFilename(ccPositionTextureColor_vert, kAlphaFragmentShaderFilename, program);
	if (program)
	{
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor,    kCCVertexAttrib_Color);
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

