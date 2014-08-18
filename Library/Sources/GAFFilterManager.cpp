#include "GAFPrecompiled.h"
#include "GAFFilterManager.h"
#include "GAFFilterData.h"
#include "GAFShaderManager.h"

#include "../external/xxhash/xxhash.h"

using namespace cocos2d;
using namespace std;

static const int kGaussianKernelSize = 9;

GAFFilterManager::Cache_t GAFFilterManager::s_cache;
GAFFilterManager* GAFFilterManager::s_instance = nullptr;
size_t GAFFilterManager::s_maxCacheSize = 1024 * 1024 * 4;

static const string k_blurName = "GaussianBlur";
static const string k_glowName = "Glow";
static const string k_shadowName = "DropShadow";

bool GAFFilterManager::init()
{
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
    {
        const char* vs = GAFShaderManager::getShader(GAFShaderManager::EVertexShader::GaussBlur);
        const char* fs = GAFShaderManager::getShader(GAFShaderManager::EFragmentShader::GaussBlur);
        GLProgram* program = GLProgram::createWithByteArrays(vs, fs);
        CCShaderCache::getInstance()->addGLProgram(program, k_blurName);
    }
    {
        const char* vs = GAFShaderManager::getShader(GAFShaderManager::EVertexShader::GaussBlur);
        const char* fs = GAFShaderManager::getShader(GAFShaderManager::EFragmentShader::Glow);
        GLProgram* program = GLProgram::createWithByteArrays(vs, fs);
        CCShaderCache::getInstance()->addGLProgram(program, k_glowName);
    }

    return true;
}

void GAFFilterManager::update(float dt)
{
    //m_policy->update(dt);
}

Texture2D* GAFFilterManager::applyFilter(cocos2d::Sprite* texture, GAFFilterData* filter)
{
    int id = hash(texture, filter);
    if(hasTexture(id))
    {

        return *s_cache[id];
    }
    else
    {
        return renderFilteredTexture(texture, filter, id);
    }
}

unsigned int GAFFilterManager::hash(Sprite* sprite, GAFFilterData* filter)
{
    struct Hash
    {
        int texture;
        Rect rect;
        GAFBlurFilterData blur;
        GAFGlowFilterData glow;
        GAFDropShadowFilterData shadow;
    };

    Hash hash;
    memset(&hash, 0, sizeof(Hash));

    hash.texture = sprite->getTexture()->getName();
    hash.rect = sprite->getTextureRect();

    if (filter->getType() == GAFFilterType::GFT_Blur)
    {
        hash.blur = *static_cast<GAFBlurFilterData*>(filter);
    }
    else if (filter->getType() == GAFFilterType::GFT_Glow)
    {
        hash.glow = *static_cast<GAFGlowFilterData*>(filter);
    }
    else if (filter->getType() == GAFFilterType::GFT_DropShadow)
    {
        hash.shadow = *static_cast<GAFDropShadowFilterData*>(filter);
    }
    
    return XXH32(&hash, sizeof(Hash), 0);
}

bool GAFFilterManager::hasTexture(unsigned int id)
{
    return s_cache.find(id) != s_cache.end();
}

Texture2D* GAFFilterManager::renderFilteredTexture(Sprite* sprite, GAFFilterData* filter)
{
    int id = hash(sprite, filter);
    return renderFilteredTexture(sprite, filter, id);
}

cocos2d::Texture2D* GAFFilterManager::renderGlowTexture(cocos2d::Sprite* sprite, GAFGlowFilterData* filter)
{
    GLProgram* program = CCShaderCache::getInstance()->getGLProgram(k_glowName);

    const float blurRadiusX = (filter->blurSize.width / 4.f);
    const float blurRadiusY = (filter->blurSize.height / 4.f);

    Size textureSize = sprite->getTextureRect().size;
    Size rTextureSize = Size(textureSize.width + kGaussianKernelSize * blurRadiusX,
                             textureSize.height + kGaussianKernelSize * blurRadiusY);

    const int rTextureW = static_cast<int>(rTextureSize.width);
    const int rTextureH = static_cast<int>(rTextureSize.height);

    BlendFunc pureBlend = { GL_ONE, GL_ZERO };

    RenderTexture* outA = RenderTexture::create(rTextureSize.width, rTextureSize.height);
    // A
    // Draw desired sprite only
    CHECK_GL_ERROR_DEBUG();
    {
        Sprite *s = Sprite::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
        s->setFlippedY(sprite->isFlippedY());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setBlendFunc(pureBlend);

        outA->beginWithClear(0, 0, 0, 0);
        s->visit();
        outA->end();
    }

    RenderTexture* outB = RenderTexture::create(rTextureW, rTextureH);
    // B
    // Draw with blur over X coordinate
    CHECK_GL_ERROR_DEBUG();
    {
        GLfloat texelWidthValue = blurRadiusX / (GLfloat)rTextureSize.width;
        GLfloat texelHeightValue = 0;

        GLProgramState* state = GLProgramState::create(program);
        state->setUniformFloat("u_texelOffset_x", texelWidthValue);
        state->setUniformFloat("u_texelOffset_y", texelHeightValue);
        state->setUniformVec4("u_glowColor", *(Vec4*)(&filter->color));

        Sprite* s = Sprite::createWithTexture(outA->getSprite()->getTexture());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setGLProgramState(state);
        s->setFlippedY(true);
        s->setBlendFunc(pureBlend);

        outB->beginWithClear(0, 0, 0, 0);
        s->visit();
        outB->end();
    }

    RenderTexture* outC = RenderTexture::create(rTextureW, rTextureH);
    // C
    // Draw with blur over Y coordinate
    CHECK_GL_ERROR_DEBUG();
    {
        GLfloat texelWidthValue = 0;
        GLfloat texelHeightValue = blurRadiusY / (GLfloat)rTextureSize.height;

        GLProgramState* state = GLProgramState::create(program);
        state->setUniformFloat("u_texelOffset_x", texelWidthValue);
        state->setUniformFloat("u_texelOffset_y", texelHeightValue);
        state->setUniformVec4("u_glowColor", *(Vec4*)(&filter->color));

        Sprite* s = Sprite::createWithTexture(outB->getSprite()->getTexture());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setGLProgramState(state);
        s->setFlippedY(true);
        s->setBlendFunc(pureBlend);

        outC->beginWithClear(0, 0, 0, 0);
        s->visit();
        outC->end();
    }
    CHECK_GL_ERROR_DEBUG();
    {
        Sprite *s = Sprite::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
        s->setFlippedY(sprite->isFlippedY());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        outC->begin();
        s->visit();
        outC->end();
    }
    CHECK_GL_ERROR_DEBUG();
    Director::getInstance()->getRenderer()->render();
    return outC->getSprite()->getTexture();
}

cocos2d::Texture2D* GAFFilterManager::renderBlurTexture(cocos2d::Sprite* sprite, GAFBlurFilterData* filter)
{
    GLProgram* program = CCShaderCache::getInstance()->getGLProgram(k_blurName);

    GAFBlurFilterData* f = static_cast<GAFBlurFilterData*>(filter);
    const float blurRadiusX = (f->blurSize.width / 4.f);
    const float blurRadiusY = (f->blurSize.height / 4.f);

    Size textureSize = sprite->getTextureRect().size;
    Size rTextureSize = Size(textureSize.width + kGaussianKernelSize * blurRadiusX,
                             textureSize.height + kGaussianKernelSize * blurRadiusY);

    const int rTextureW = static_cast<int>(rTextureSize.width);
    const int rTextureH = static_cast<int>(rTextureSize.height);

    BlendFunc pureBlend = { GL_ONE, GL_ZERO };

    RenderTexture* outA = RenderTexture::create(rTextureSize.width, rTextureSize.height);
    // A
    // Draw desired sprite only
    CHECK_GL_ERROR_DEBUG();
    {
        Sprite *s = Sprite::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
        s->setFlippedY(sprite->isFlippedY());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setBlendFunc(pureBlend);

        outA->beginWithClear(0, 0, 0, 0);
        s->visit();
        outA->end();
    }

    RenderTexture* outB = RenderTexture::create(rTextureW, rTextureH);
    // B
    // Draw with blur over X coordinate
    CHECK_GL_ERROR_DEBUG();
    {
        GLfloat texelWidthValue = blurRadiusX / (GLfloat)rTextureSize.width;
        GLfloat texelHeightValue = 0;

        GLProgramState* state = GLProgramState::create(program);
        state->setUniformFloat("u_texelOffset_x", texelWidthValue);
        state->setUniformFloat("u_texelOffset_y", texelHeightValue);

        Sprite* s = Sprite::createWithTexture(outA->getSprite()->getTexture());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setGLProgramState(state);
        s->setFlippedY(true);
        s->setBlendFunc(pureBlend);

        outB->beginWithClear(0, 0, 0, 0);
        s->visit();
        outB->end();
    }

    RenderTexture* outC = RenderTexture::create(rTextureW, rTextureH);
    // C
    // Draw with blur over Y coordinate
    CHECK_GL_ERROR_DEBUG();
    {
        GLfloat texelWidthValue = 0;
        GLfloat texelHeightValue = blurRadiusY / (GLfloat)rTextureSize.height;

        GLProgramState* state = GLProgramState::create(program);
        state->setUniformFloat("u_texelOffset_x", texelWidthValue);
        state->setUniformFloat("u_texelOffset_y", texelHeightValue);

        Sprite* s = Sprite::createWithTexture(outB->getSprite()->getTexture());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setGLProgramState(state);
        s->setFlippedY(true);
        s->setBlendFunc(pureBlend);

        outC->beginWithClear(0, 0, 0, 0);
        s->visit();
        outC->end();
    }
    CHECK_GL_ERROR_DEBUG();

    Director::getInstance()->getRenderer()->render();
    return outC->getSprite()->getTexture();
}

cocos2d::Texture2D* GAFFilterManager::renderShadowTexture(cocos2d::Sprite* sprite, GAFDropShadowFilterData* filter)
{
    GLProgram* program = CCShaderCache::getInstance()->getGLProgram(k_glowName);

    const float blurRadiusX = (filter->blurSize.width / 4.f);
    const float blurRadiusY = (filter->blurSize.height / 4.f);

    Size textureSize = sprite->getTextureRect().size;
    Size rTextureSize = Size(textureSize.width + kGaussianKernelSize * blurRadiusX,
                             textureSize.height + kGaussianKernelSize * blurRadiusY);

    const int rTextureW = static_cast<int>(rTextureSize.width);
    const int rTextureH = static_cast<int>(rTextureSize.height);

    BlendFunc pureBlend = { GL_ONE, GL_ZERO };

    RenderTexture* outA = RenderTexture::create(rTextureSize.width, rTextureSize.height);
    // A
    // Draw desired sprite only
    CHECK_GL_ERROR_DEBUG();
    {
        Sprite *s = Sprite::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
        s->setFlippedY(sprite->isFlippedY());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setBlendFunc(pureBlend);

        outA->beginWithClear(0, 0, 0, 0);
        s->visit();
        outA->end();
    }

    RenderTexture* outB = RenderTexture::create(rTextureW, rTextureH);
    // B
    // Draw with blur over X coordinate
    CHECK_GL_ERROR_DEBUG();
    {
        GLfloat texelWidthValue = blurRadiusX / (GLfloat)rTextureSize.width;
        GLfloat texelHeightValue = 0;

        GLProgramState* state = GLProgramState::create(program);
        state->setUniformFloat("u_texelOffset_x", texelWidthValue);
        state->setUniformFloat("u_texelOffset_y", texelHeightValue);
        state->setUniformVec4("u_glowColor", *(Vec4*)(&filter->color));

        Sprite* s = Sprite::createWithTexture(outA->getSprite()->getTexture());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setGLProgramState(state);
        s->setFlippedY(true);
        s->setBlendFunc(pureBlend);

        outB->beginWithClear(0, 0, 0, 0);
        s->visit();
        outB->end();
    }

    RenderTexture* outC = RenderTexture::create(rTextureW, rTextureH);
    // C
    // Draw with blur over Y coordinate
    CHECK_GL_ERROR_DEBUG();
    {
        GLfloat texelWidthValue = 0;
        GLfloat texelHeightValue = blurRadiusY / (GLfloat)rTextureSize.height;

        GLProgramState* state = GLProgramState::create(program);
        state->setUniformFloat("u_texelOffset_x", texelWidthValue);
        state->setUniformFloat("u_texelOffset_y", texelHeightValue);
        state->setUniformVec4("u_glowColor", *(Vec4*)(&filter->color));

        Sprite* s = Sprite::createWithTexture(outB->getSprite()->getTexture());
        s->setPosition(rTextureSize.width / 2, rTextureSize.height / 2);
        s->setGLProgramState(state);
        s->setFlippedY(true);
        s->setBlendFunc(pureBlend);

        outC->beginWithClear(0, 0, 0, 0);
        s->visit();
        outC->end();
    }
    Director::getInstance()->getRenderer()->render();
    return outC->getSprite()->getTexture();
}

Texture2D* GAFFilterManager::renderFilteredTexture(Sprite* sprite, GAFFilterData* filter, unsigned int id)
{
    Texture2D* texture = nullptr;
    if (filter->getType() == GAFFilterType::GFT_Blur)
    {
        texture = renderBlurTexture(sprite, static_cast<GAFBlurFilterData*>(filter));
    }
    else if (filter->getType() == GAFFilterType::GFT_Glow)
    {
        texture = renderGlowTexture(sprite, static_cast<GAFGlowFilterData*>(filter));
    }
    else if (filter->getType() == GAFFilterType::GFT_DropShadow)
    {
        texture = renderShadowTexture(sprite, static_cast<GAFDropShadowFilterData*>(filter));
    }
    else
    {
        CCASSERT(false, "Errorr. No filter processor found!");
    }

    insertTexture(texture, id);

    return texture;
}

void GAFFilterManager::insertTexture(cocos2d::Texture2D* texture, unsigned int id)
{
    CCASSERT(texture, "Failed to filter texture");
    s_cache[id] = texture;
    
    //m_policy->onInserted(id);
    
    if(s_cache.size() == 1)
    {
        return;
    }
    
    size_t totalSize = 0;
    for(auto cache : s_cache)
    {
        totalSize += cache.second.memoryUsed();
    }
    
    if(totalSize > s_maxCacheSize)
    {
        // Delete one texture
        auto later = s_cache.begin();
        time_t time = later->second.lastUsed();
        for(auto cache = ++s_cache.begin(), end = s_cache.end(); cache != end; ++cache)
        {
            if( cache->second.lastUsed() < time )
            {
                later = cache;
                time = cache->second.lastUsed();
            }
        }
        s_cache.erase(later);
    
    }
    
}

GAFFilterManager* GAFFilterManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new GAFFilterManager();
        s_instance->init();
    }
    return s_instance;
}
