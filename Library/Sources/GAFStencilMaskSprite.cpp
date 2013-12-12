#include "GAFStencilMaskSprite.h"
#include "GAFShaderManager.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccShaders.h"
#include <algorithm>

static const char * kPCStencilMaskAlphaFilterFragmentShaderFilename = "Shaders/pcShader_PositionTexture_alphaFilter.fs";


static bool compare_stencil_sprites(const void* p1, const void* p2)
{
	GAFSprite* sp1 = (GAFSprite*)p1;
	GAFSprite* sp2 = (GAFSprite*)p2;
	
	return sp1->getZOrder() < sp2->getZOrder();
}

GAFStencilMaskSprite::GAFStencilMaskSprite()
:
_maskedObjects(NULL),
_isReorderMaskedObjectsDirty(false)
{
}

static std::map<CCNode *, GAFStencilMaskSprite *> _object2maskedContainer;

void GAFStencilMaskSprite::updateMaskContainerOf(CCNode * node)
{
	std::map<CCNode *, GAFStencilMaskSprite *>::iterator it = _object2maskedContainer.find(node);
	if (it != _object2maskedContainer.end())
	{
		it->second->invalidateMaskedObjectsOrder();
	}
}


bool GAFStencilMaskSprite::initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated)
{
	if (!GAFSprite::initWithTexture(pTexture, CCRectMake(0, 0, rect.size.width, rect.size.height), rotated))
	{
		return false;
	}
	CC_SAFE_RELEASE(_maskedObjects);
	_maskedObjects = new CCArray();
	setShaderProgram(programShaderForMask());
	_isReorderMaskedObjectsDirty = false;
	return true;
}

GAFStencilMaskSprite::~GAFStencilMaskSprite()
{
	if (_maskedObjects)
	{
		for (int i = 0; i < _maskedObjects->count(); ++i)
		{
			CCNode * node = (CCNode*)_maskedObjects->objectAtIndex(i);
			std::map<CCNode *, GAFStencilMaskSprite *>::iterator it = _object2maskedContainer.find(node);
			if (it != _object2maskedContainer.end())
			{
				_object2maskedContainer.erase(it);
			}
		}
	}
	
	CC_SAFE_RELEASE(_maskedObjects);
}


void GAFStencilMaskSprite::visit()
{
	GAFSprite::visit();    
	sortAllMaskedObjects();
    // Draw subobjects, assuming mask and object are on the same layer
	for (int i = 0; i < _maskedObjects->count(); ++i)
	{
		CCNode * object = (CCNode *) _maskedObjects->objectAtIndex(i);
		object->visit();
	}
	glDisable(GL_STENCIL_TEST);
}

void GAFStencilMaskSprite::sortAllMaskedObjects()
{
	if (_isReorderMaskedObjectsDirty)
	{
		std::sort(_maskedObjects->data->arr,
				  _maskedObjects->data->arr + _maskedObjects->data->num,
				  compare_stencil_sprites);
		_isReorderMaskedObjectsDirty = false;
	}
}

void GAFStencilMaskSprite::draw()
{
	// Prepare stencil
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 1);
    
    // Draw mask
	GAFSprite::draw();
    
    // Use stencil
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 1);
}

void GAFStencilMaskSprite::addMaskedObject(CCNode * anObject)
{	
	std::map<CCNode *, GAFStencilMaskSprite *>::iterator it = _object2maskedContainer.find(anObject);
	
	GAFStencilMaskSprite * maskContainer = it != _object2maskedContainer.end() ? it->second : NULL;
	
	if (maskContainer)
	{
		maskContainer->removeMaskedObject(anObject);
	}
	
	it = _object2maskedContainer.find(anObject);
	
	maskContainer = it != _object2maskedContainer.end() ? it->second : NULL;
	
	if (maskContainer != this)
	{
		_object2maskedContainer[anObject] = this;
		_maskedObjects->addObject(anObject);
		_isReorderMaskedObjectsDirty = true;
	}
}

void GAFStencilMaskSprite::removeMaskedObject(CCNode * anObject)
{
	if (_maskedObjects->containsObject(anObject))
	{
		std::map<CCNode *, GAFStencilMaskSprite *>::iterator it = _object2maskedContainer.find(anObject);
		CCAssert(it != _object2maskedContainer.end(), "iterator must be valid");
		_object2maskedContainer.erase(it);
		_maskedObjects->removeObject(anObject);
		_isReorderMaskedObjectsDirty = true;
	}
}

void GAFStencilMaskSprite::invalidateMaskedObjectsOrder()
{
	_isReorderMaskedObjectsDirty = true;
}

CCGLProgram * GAFStencilMaskSprite::programShaderForMask()
{
	CCGLProgram *program = CCShaderCache::sharedShaderCache()->programForKey(kGAFStencilMaskAlphaFilterProgramCacheKey);
	
    if (!program)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && !defined(_DEBUG))
		#include "ShadersPrecompiled/GAFPrecompiledShaders.h"
		program = new CCGLProgram();
		program->autorelease();
		program->initWithPrecompiledProgramByteArray((const GLchar*)kGAFScrollLayerAlphaFilterProgramCacheKey, 
			sizeof(kGAFScrollLayerAlphaFilterProgramCacheKey));
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor,    kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->updateUniforms();
		CHECK_GL_ERROR_DEBUG();
		CCShaderCache::sharedShaderCache()->addProgram(program, kGAFStencilMaskAlphaFilterProgramCacheKey);
#else
        program = GAFShaderManager::createWithFragmentFilename(ccPositionTextureColor_vert, kPCStencilMaskAlphaFilterFragmentShaderFilename);
        if (program)
        {
			program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			program->addAttribute(kCCAttributeNameColor,    kCCVertexAttrib_Color);
			program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
            program->link();
			program->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
			CCShaderCache::sharedShaderCache()->addProgram(program, kGAFStencilMaskAlphaFilterProgramCacheKey);
        }
        else
        {
            CCLOGERROR("Cannot load program for programShaderForMask.");
            return NULL;
        }
#endif
    }
	
    program->use();
	return program;
}

#if CC_ENABLE_CACHE_TEXTURE_DATA
void _GAFreloadStencilShader()
{
	CCGLProgram * program = CCShaderCache::sharedShaderCache()->programForKey(kGAFStencilMaskAlphaFilterProgramCacheKey);
	
	if (!program)
	{
		return;
	}
	program->reset();
	program = GAFShaderManager::createWithFragmentFilename(ccPositionTextureColor_vert, kPCStencilMaskAlphaFilterFragmentShaderFilename, program);
	if (program)
	{
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor,    kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		CHECK_GL_ERROR_DEBUG();
		CCLOGERROR("GAFStencilMaskSprite RELOADED");
	}
	else
	{
		CCAssert(false, "Can not RELOAD programShaderForMask");
	}
}
#endif
