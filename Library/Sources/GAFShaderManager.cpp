#include "GAFPrecompiled.h"
#include "GAFShaderManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
namespace gps
{
#include "ShadersPrecompiled/GAFPrecompiledShaders.h"
}
#include "CCPrecompiledShaders.h"
bool GAFShaderManager::s_initialized = false;
#endif

using std::string;

#define STRINGIFY(A)  #A

#include "Shaders/GaussianBlurFragmentShader.frag"
#include "Shaders/GaussianBlurVertexShader.vert"
#include "Shaders/GlowFragmentShader.frag"
#include "Shaders/pcShader_masked_texture.frag"
#include "Shaders/pcShader_PositionTexture_alphaFilter.frag"
#include "Shaders/pcShader_PositionTextureAlpha_frag.frag"
#include "Shaders/pcShader_PositionTextureAlpha_frag_noCTX.frag"

namespace gaf
{

    const char * const GAFShaderManager::s_fragmentShaders[] =
    {
        GaussianBlurFragmentShader_fs,
        GlowFragmentShader_fs,
        pcShader_masked_texture_fs,
        pcShader_PositionTexture_alphaFilter_fs,
        pcShader_PositionTextureAlpha_frag_fs,
        pcShader_PositionTextureAlpha_frag_noCTX_fs
    };

    const char* const GAFShaderManager::s_vertexShaders[] =
    {
        GaussianBlurVertexShader_vs
    };

    void GAFShaderManager::handleEnterBackground()
    {
        // Stub yet
    }

    void GAFShaderManager::Initialize(bool force /*= false*/)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        bool skip = !force && s_initialized;
        if (!skip)
        {
            for (unsigned int i = 0; i < gps::s_numPrograms; ++i)
            {
                cocos2d::
                    CCPrecompiledShaders::getInstance()->addPrecompiledProgram(
                    gps::s_programKeys[i],
                    gps::s_programs[i],
                    gps::s_programLengths[i]);
            }
            s_initialized = true;
        }
#endif
    }
}
