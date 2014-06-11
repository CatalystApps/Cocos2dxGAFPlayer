#include "GAFPrecompiled.h"
#include "GAFStencilMaskSprite.h"
#include "GAFShaderManager.h"

static const char * kPCStencilMaskAlphaFilterFragmentShaderFilename = "Shaders/pcShader_PositionTexture_alphaFilter.fs";

#define USE_LAYERED_STENCIL 1


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
#if CC_ENABLE_CACHE_TEXTURE_DATA
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
        callfuncO_selector(GAFStencilMaskSprite::listenToForeground),
        EVENT_COME_TO_FOREGROUND,
        NULL);
#endif
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

#if CC_ENABLE_CACHE_TEXTURE_DATA
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
#endif
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
    glStencilFunc(m_stencilState.currentStencilFunc, m_stencilState.currentStencilRef, m_stencilState.currentStencilValueMask);
    glStencilOp(m_stencilState.currentStencilFail, m_stencilState.currentStencilPassDepthFail, m_stencilState.currentStencilPassDepthPass);
    glStencilMask(m_stencilState.currentStencilWriteMask);

    if (!m_stencilState.currentStencilEnabled)
    {
        glDisable(GL_STENCIL_TEST);
    }

    // we are done using this layer, decrement
    m_stencilLayer = std::max(--m_stencilLayer, -1);
}

void GAFStencilMaskSprite::draw()
{
    // Prepare stencil
#if USE_LAYERED_STENCIL
    m_stencilLayer = std::min(++m_stencilLayer, 255);
    // mask of the current layer (ie: for layer 3: 00000100)
    GLint mask_layer = 0x1 << m_stencilLayer;
    // mask of all layers less than the current (ie: for layer 3: 00000011)
    GLint mask_layer_l = mask_layer - 1;
    // mask of all layers less than or equal to the current (ie: for layer 3: 00000111)
    GLint mask_layer_le = mask_layer | mask_layer_l;

    // manually save the stencil state

    m_stencilState.currentStencilEnabled = GL_FALSE;
    m_stencilState.currentStencilWriteMask = UINT_MAX;
    m_stencilState.currentStencilFunc = GL_ALWAYS;
    m_stencilState.currentStencilRef = 0;
    m_stencilState.currentStencilValueMask = UINT_MAX;
    m_stencilState.currentStencilFail = GL_KEEP;
    m_stencilState.currentStencilPassDepthFail = GL_KEEP;
    m_stencilState.currentStencilPassDepthPass = GL_KEEP;
    m_stencilState.currentStencilEnabled = glIsEnabled(GL_STENCIL_TEST);


    glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint *)&m_stencilState.currentStencilWriteMask);
    glGetIntegerv(GL_STENCIL_FUNC, (GLint *)&m_stencilState.currentStencilFunc);
    glGetIntegerv(GL_STENCIL_REF, &m_stencilState.currentStencilRef);
    glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint *)&m_stencilState.currentStencilValueMask);
    glGetIntegerv(GL_STENCIL_FAIL, (GLint *)&m_stencilState.currentStencilFail);
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint *)&m_stencilState.currentStencilPassDepthFail);
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint *)&m_stencilState.currentStencilPassDepthPass);


    // enable stencil use
    glEnable(GL_STENCIL_TEST);
    // check for OpenGL error while enabling stencil test

    CHECK_GL_ERROR_DEBUG();

    // all bits on the stencil buffer are readonly, except the current layer bit,
    // this means that operation like glClear or glStencilOp will be masked with this value

    glStencilMask(mask_layer);

    glClear(GL_STENCIL_BUFFER_BIT);

    // manually save the depth test state
    //GLboolean currentDepthTestEnabled = GL_TRUE;
    GLboolean currentDepthWriteMask = GL_TRUE;
    //currentDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    glGetBooleanv(GL_DEPTH_WRITEMASK, &currentDepthWriteMask);


    // disable depth test while drawing the stencil
    //glDisable(GL_DEPTH_TEST);
    // disable update to the depth buffer while drawing the stencil,
    // as the stencil is not meant to be rendered in the real scene,
    // it should never prevent something else to be drawn,
    // only disabling depth buffer update should do

    glDepthMask(GL_FALSE);

    ///////////////////////////////////
    // CLEAR STENCIL BUFFER


    // manually clear the stencil buffer by drawing a fullscreen rectangle on it
    // setup the stencil test func like this:
    // for each pixel in the fullscreen rectangle
    //     never draw it into the frame buffer
    //     if not in inverted mode: set the current layer value to 0 in the stencil buffer
    //     if in inverted mode: set the current layer value to 1 in the stencil buffer

    glStencilFunc(GL_NEVER, mask_layer, mask_layer);
    glStencilOp(GL_ZERO, GL_KEEP, GL_KEEP);


    // draw a fullscreen solid rectangle to clear the stencil buffer
    //ccDrawSolidRect(CCPointZero, ccpFromSize([[CCDirector sharedDirector] winSize]), ccc4f(1, 1, 1, 1));

    ccDrawSolidRect(CCPointZero, ccpFromSize(CCDirector::sharedDirector()->getWinSize()), ccc4f(1, 1, 1, 1));


    ///////////////////////////////////
    // DRAW CLIPPING STENCIL

    // setup the stencil test func like this:
    // for each pixel in the stencil node
    //     never draw it into the frame buffer
    //     if not in inverted mode: set the current layer value to 1 in the stencil buffer
    //     if in inverted mode: set the current layer value to 0 in the stencil buffer

    glStencilFunc(GL_NEVER, mask_layer, mask_layer);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

    // Draw mask

    GAFSprite::draw();

    // setup the stencil test func like this:
    // for each pixel of this node and its childs
    //     if all layers less than or equals to the current are set to 1 in the stencil buffer
    //         draw the pixel and keep the current layer in the stencil buffer
    //     else
    //         do not draw the pixel but keep the current layer in the stencil buffer

    glStencilFunc(GL_EQUAL, mask_layer_le, mask_layer_le);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


#else
    glEnable(GL_STENCIL_TEST);

    glClear(GL_STENCIL_BUFFER_BIT);
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

void GAFStencilMaskSprite::updateStencilLayer( int newLayer )
{
    m_stencilLayer = newLayer;
}

void GAFStencilMaskSprite::listenToForeground( CCObject* )
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
