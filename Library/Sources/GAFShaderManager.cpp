#include "GAFPrecompiled.h"
#include "GAFShaderManager.h"

using std::string;

#define STRINGIFY(A)  #A

#include "Shaders/GaussianBlurFragmentShader.frag"
#include "Shaders/GaussianBlurVertexShader.vert"
#include "Shaders/GlowFragmentShader.frag"
#include "Shaders/GlowVertexShader.vert"
#include "Shaders/pcBlurredSprite_Shader_HorizontalBlur.frag"
#include "Shaders/pcBlurredSprite_Shader_VerticalBlur.frag"
#include "Shaders/pcShader_masked_texture.frag"
#include "Shaders/pcShader_PositionTexture_alphaFilter.frag"
#include "Shaders/pcShader_PositionTextureAlpha_frag.frag"
#include "Shaders/pcShader_PositionTextureAlpha_frag_noCTX.frag"

const char* GAFShaderManager::getShader(const std::string& name)
{
    if (name == "GaussianBlurFragmentShader.fs")
    {
        return GaussianBlurFragmentShader_fs;
    }
    else if (name == "GaussianBlurVertexShader.vs")
    {
        return GaussianBlurVertexShader_vs;
    }
    else if (name == "GlowFragmentShader.fs")
    {
        return GlowFragmentShader_fs;
    }
    else if (name == "GlowVertexShader.vs")
    {
        return GlowVertexShader_vs;
    }
    else if (name == "pcBlurredSprite_Shader_HorizontalBlur.fs")
    {
        return pcBlurredSprite_Shader_HorizontalBlur_fs;
    }
    else if (name == "pcBlurredSprite_Shader_VerticalBlur.fs")
    {
        return pcBlurredSprite_Shader_VerticalBlur_fs;
    }
    else if (name == "pcShader_masked_texture.fs")
    {
        return pcShader_masked_texture_fs;
    }
    else if (name == "pcShader_PositionTexture_alphaFilter.fs")
    {
        return pcShader_PositionTexture_alphaFilter_fs;
    }
    else if (name == "pcShader_PositionTextureAlpha_frag.fs")
    {
        return pcShader_PositionTextureAlpha_frag_fs;
    }
    else if (name == "pcShader_PositionTextureAlpha_frag_noCTX.fs")
    {
        return pcShader_PositionTextureAlpha_frag_noCTX_fs;
    }
    else
    {
        CCASSERT(false, "No shader with given name found.");
        return nullptr;
    }
}

cocos2d::GLProgram * GAFShaderManager::createWithFragmentFilename(const char * vertexSource, const char * fragmentFilename, cocos2d::GLProgram * p)
{
    if (!vertexSource || !fragmentFilename)
    {
        return NULL;
    }

    std::string fileContent = GAFShaderManager::getShader(string(fragmentFilename));

    if (fileContent.empty())
    {
        CCLOGERROR("Cannot load fragment shader with name %s", fragmentFilename);
        return NULL;
    }
    cocos2d::GLProgram * res;

    if (p)
    {
        res = p;
    }
    else
    {
        res = new cocos2d::GLProgram();
    }

    if (!res)
    {
        return NULL;
    }
    
    if (!res->initWithByteArrays(vertexSource, fileContent.c_str()))
    {
        CC_SAFE_RELEASE(res);
        return NULL;
    }

    if (!p)
    {
        res->autorelease();
    }

    return res;
}

void GAFShaderManager::handleEnterBackground()
{
    // Stub yet
}
