#include "GAFPrecompiled.h"
#include "GAFStencilMaskSprite.h"
#include "GAFShaderManager.h"

static const char * kPCStencilMaskAlphaFilterFragmentShaderFilename = "Shaders/pcShader_PositionTexture_alphaFilter.fs";

#define USE_LAYERED_STENCIL 0


static bool compare_stencil_sprites(const void* p1, const void* p2)
{
    GAFSprite* sp1 = (GAFSprite*)p1;
    GAFSprite* sp2 = (GAFSprite*)p2;

    return sp1->getZOrder() < sp2->getZOrder();
}

GAFStencilMaskSprite::GAFStencilMaskSprite(int stencilLayer)
:
_maskedObjects(NULL),
_isReorderMaskedObjectsDirty(false),
m_stencilLayer(stencilLayer)
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
        for (unsigned int i = 0; i < _maskedObjects->count(); ++i)
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
    for (unsigned int i = 0; i < _maskedObjects->count(); ++i)
    {
        CCNode * object = (CCNode *)_maskedObjects->objectAtIndex(i);
        object->visit();
    }
#if USE_LAYERED_STENCIL
    _disableStencil();
#else
    glDisable(GL_STENCIL_TEST);
#endif
    
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

void GAFStencilMaskSprite::_setupStencilForMask()
{
    if (m_stencilLayer == 0)
    {
        glEnable(GL_STENCIL_TEST);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    //glStencilMask(0xFF);

    // Draw mask
    GAFSprite::draw();

    //glStencilMask(0x00);
}

void GAFStencilMaskSprite::_setupStencilForContent()
{
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    
    glStencilFunc(GL_LEQUAL, ++m_stencilLayer, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void GAFStencilMaskSprite::_disableStencil()
{
    m_stencilLayer = std::max(--m_stencilLayer, 0);

    if (m_stencilLayer == 0)
    {
        glDisable(GL_STENCIL_TEST);
    }
}

void GAFStencilMaskSprite::draw()
{
    // Prepare stencil
#if USE_LAYERED_STENCIL
    _setupStencilForMask();
    _setupStencilForContent();
#else
     glEnable(GL_STENCIL_TEST);
     glClear(GL_STENCIL_BUFFER_BIT);
 
     glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
     glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
     glStencilFunc(GL_ALWAYS, 1, 1);
 
     // Draw mask
     GAFSprite::draw();
 
     // Use stencil
     glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
     glStencilFunc(GL_EQUAL, 1, 1);
#endif
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
        program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        program->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        CCShaderCache::sharedShaderCache()->addProgram(program, kGAFStencilMaskAlphaFilterProgramCacheKey);
#else
        program = GAFShaderManager::createWithFragmentFilename(ccPositionTextureColor_vert, kPCStencilMaskAlphaFilterFragmentShaderFilename);
        if (program)
        {
            program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
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
        program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
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
