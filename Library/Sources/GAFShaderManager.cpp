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

const char * const GAFShaderManager::s_fragmentShaders[] =
{
    GaussianBlurFragmentShader_fs,
    GlowFragmentShader_fs,
    pcBlurredSprite_Shader_HorizontalBlur_fs,
    pcBlurredSprite_Shader_VerticalBlur_fs,
    pcShader_masked_texture_fs,
    pcShader_PositionTexture_alphaFilter_fs,
    pcShader_PositionTextureAlpha_frag_fs,
    pcShader_PositionTextureAlpha_frag_noCTX_fs
};

const char* const GAFShaderManager::s_vertexShaders[] =
{
    GaussianBlurVertexShader_vs,
    GlowVertexShader_vs,
};

void GAFShaderManager::handleEnterBackground()
{
    // Stub yet
}
