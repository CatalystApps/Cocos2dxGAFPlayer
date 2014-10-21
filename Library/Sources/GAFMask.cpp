#include "GAFPrecompiled.h"
#include "GAFMask.h"
#include "GAFShaderManager.h"
#include "base/ccMacros.h"

#define USE_LAYERED_STENCIL 0

NS_GAF_BEGIN

GAFMask::Object2maskedContainer_t GAFMask::object2maskedContainer;

void GAFMask::_sortAllMaskedObjects()
{
    if (m_isReorderMaskedObjectsDirty)
    {
        std::sort(m_maskedObjects->begin(), m_maskedObjects->end(), [](const cocos2d::Ref* p1, const cocos2d::Ref* p2)->bool
        {
            const GAFSprite* spr1 = static_cast<const GAFSprite*>(p1);
            const GAFSprite* spr2 = static_cast<const GAFSprite*>(p2);
            return spr1->getLocalZOrder() < spr2->getLocalZOrder();
        });

        m_isReorderMaskedObjectsDirty = false;
    }
}

void GAFMask::_disableStencil()
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

void GAFMask::beginStencil(cocos2d::Mat4& transform)
{
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 1);

    GAFSprite::customDraw(transform);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 1);
}

void GAFMask::endStencil()
{
#if USE_LAYERED_STENCIL
    _disableStencil();
#else
    glDisable(GL_STENCIL_TEST);
#endif
}

GAFMask::GAFMask(int stencilLayer):
m_stencilLayer(stencilLayer),
m_isReorderMaskedObjectsDirty(true),
m_maskedObjects(nullptr)
{
    m_charType = GAFCharacterType::Texture;
}

GAFMask::~GAFMask()
{
    if (m_maskedObjects)
    {
        for (int i = 0; i < m_maskedObjects->count(); ++i)
        {
            cocos2d::Node * node = (cocos2d::Node*)m_maskedObjects->getObjectAtIndex(i);
            object2maskedContainer.erase(node);
        }
    }

    CC_SAFE_RELEASE(m_maskedObjects);
}

bool GAFMask::initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated)
{
    if (!GAFSprite::initWithTexture(pTexture, cocos2d::Rect(0, 0, rect.size.width, rect.size.height), rotated))
    {
        return false;
    }

    CC_SAFE_RELEASE(m_maskedObjects);
    m_maskedObjects = cocos2d::__Array::create();
    m_maskedObjects->retain();
    setGLProgram(programShaderForMask());
    return true;
}

cocos2d::GLProgram * GAFMask::programShaderForMask()
{
    cocos2d::GLProgram *program = cocos2d::ShaderCache::getInstance()->getGLProgram(kGAFStencilMaskAlphaFilterProgramCacheKey);

    if (!program)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && !defined(_DEBUG))
#include "ShadersPrecompiled/GAFPrecompiledShaders.h"
        program = cocos2d::GLProgram::createWithPrecompiledProgramByteArray(kGAFScrollLayerAlphaFilterProgramCacheKey, kGAFScrollLayerAlphaFilterProgramCacheKey);
        program->addAttribute(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
        program->addAttribute(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
        program->addAttribute(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        program->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        cocos2d::ShaderCache::getInstance()->addGLProgram(program, kGAFStencilMaskAlphaFilterProgramCacheKey);
#else
        const char* frag = GAFShaderManager::getShader(GAFShaderManager::EFragmentShader::AlphaFilter);
        program = cocos2d::GLProgram::createWithByteArrays(cocos2d::ccPositionTextureColor_vert, frag);
        if (program)
        {
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
            program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);

            program->link();
            program->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
            cocos2d::ShaderCache::getInstance()->addGLProgram(program, kGAFStencilMaskAlphaFilterProgramCacheKey);
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

#if COCOS2D_VERSION < 0x00030200
void GAFMask::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
{
#else
void GAFMask::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    (void)flags;
#endif

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

    cocos2d::DrawPrimitives::drawSolidRect(cocos2d::Vect::ZERO, cocos2d::Vect(cocos2d::Director::getInstance()->getWinSize()), cocos2d::Color4F(1, 1, 1, 1));


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

    GAFSprite::draw(renderer, transform, transformUpdated);

    // setup the stencil test func like this:
    // for each pixel of this node and its childs
    //     if all layers less than or equals to the current are set to 1 in the stencil buffer
    //         draw the pixel and keep the current layer in the stencil buffer
    //     else
    //         do not draw the pixel but keep the current layer in the stencil buffer

    glStencilFunc(GL_EQUAL, mask_layer_le, mask_layer_le);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


#else

    m_customCommand.init(-FLT_MAX);
    m_customCommand.func = CC_CALLBACK_0(GAFMask::beginStencil, this, transform);
    m_customCommand2.init(FLT_MAX);
    m_customCommand2.func = CC_CALLBACK_0(GAFMask::endStencil, this);

    renderer->addCommand(&m_customCommand);
    renderer->addCommand(&m_customCommand2);
    // Use stencil

#endif
}

#if COCOS2D_VERSION < 0x00030200
void GAFMask::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
#else
void GAFMask::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _sortAllMaskedObjects();

    m_group.init(getGlobalZOrder());
    renderer->addCommand(&m_group);
    renderer->pushGroup(m_group.getRenderQueueID());

#if COCOS2D_VERSION < 0x00030200
    GAFSprite::visit(renderer, transform, true);
#else
    GAFSprite::visit(renderer, transform, flags | cocos2d::Node::FLAGS_TRANSFORM_DIRTY);
#endif

    for (int i = 0; i < m_maskedObjects->count(); ++i)
    {
        cocos2d::Node * object = (cocos2d::Node *)m_maskedObjects->getObjectAtIndex(i);
#if COCOS2D_VERSION < 0x00030200
        object->visit(renderer, transform, transformUpdated

            );
#else
        object->visit(renderer, transform, flags);
#endif
    }
    renderer->popGroup();
}
#endif

void GAFMask::addMaskedObject(cocos2d::Node * anObject)
{
    Object2maskedContainer_t::iterator it = object2maskedContainer.find(anObject);

    GAFMask* maskContainer = it != object2maskedContainer.end() ? it->second : NULL;

    if (maskContainer)
    {
        maskContainer->removeMaskedObject(anObject);
    }

    it = object2maskedContainer.find(anObject);

    maskContainer = it != object2maskedContainer.end() ? it->second : NULL;

    if (maskContainer != this)
    {
        object2maskedContainer[anObject] = this;
        m_maskedObjects->addObject(anObject);
        m_isReorderMaskedObjectsDirty = true;
    }
}

void GAFMask::removeMaskedObject(cocos2d::Node * anObject)
{
    if (m_maskedObjects->containsObject(anObject))
    {
        Object2maskedContainer_t::iterator it = object2maskedContainer.find(anObject);
        CCAssert(it != object2maskedContainer.end(), "iterator must be valid");
        object2maskedContainer.erase(it);
        m_maskedObjects->removeObject(anObject);
        m_isReorderMaskedObjectsDirty = true;
    }
}

void GAFMask::updateStencilLayer(int newLayer)
{
    m_stencilLayer = newLayer;
}

NS_GAF_END