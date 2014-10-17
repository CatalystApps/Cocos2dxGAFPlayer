#include "GAFPrecompiled.h"
#include "GAFShaderManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
namespace gps
{
#include "ShadersPrecompiled/GAFPrecompiledShaders.h"
}
#include "CCPrecompiledShaders.h"
#endif

using std::string;
using namespace cocos2d;

#define STRINGIFY(A)  #A

#include "Shaders/GaussianBlurFragmentShader.frag"
#include "Shaders/GaussianBlurVertexShader.vert"
#include "Shaders/GlowFragmentShader.frag"
#include "Shaders/pcShader_masked_texture.frag"
#include "Shaders/pcShader_PositionTexture_alphaFilter.frag"
#include "Shaders/pcShader_PositionTextureAlpha_frag.frag"
#include "Shaders/pcShader_PositionTextureAlpha_frag_noCTX.frag"


    bool GAFShaderManager::s_initialized = false;

    const char * const GAFShaderManager::s_fragmentShaders[] =
    {
        GaussianBlurFragmentShader_fs,                  // GaussBlur
        GlowFragmentShader_fs,                          // Glow
        pcShader_masked_texture_fs,                     // Masked
        pcShader_PositionTexture_alphaFilter_fs,        // AlphaFilter
        pcShader_PositionTextureAlpha_frag_fs,          // Alpha
        pcShader_PositionTextureAlpha_frag_noCTX_fs     // AlphaNoCtx
    };

    const char* const GAFShaderManager::s_vertexShaders[] =
    {
        GaussianBlurVertexShader_vs                     // GaussBlur
    };

    cocos2d::GLProgram* GAFShaderManager::s_programs[] =
    {
        nullptr
    };
    
    const char* const GAFShaderManager::s_uniformNames[] = 
    {
        "colorTransformMult",       // ColorTransformMult
        "colorTransformOffsets",    // ColorTransformOffset
        "colorMatrix",              // ColorMatrixBody
        "colorMatrix2",             // ColorMatrixAppendix
    
        "fragmentAlpha",            // Alpha
    };

    GLint GAFShaderManager::s_uniformLocations[] =
    {
        -1
    };



    void GAFShaderManager::handleEnterBackground()
    {
        
    }



    void GAFShaderManager::Initialize(bool force /*= false*/)
    {
        bool skip = !force && s_initialized;
        if (!skip)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
            for (unsigned int i = 0; i < gps::s_numPrograms; ++i)
            {
                cocos2d::
                    CCPrecompiledShaders::getInstance()->addPrecompiledProgram(
                    gps::s_programKeys[i],
                    gps::s_programs[i],
                    gps::s_programLengths[i]);
            }
#endif

            // Alpha
            {
                const char* fragmentShader = getShader(EFragmentShader::Alpha);
                GLProgram* program = GLProgram::createWithByteArrays(cocos2d::ccPositionTextureColor_vert, fragmentShader);
                program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
                program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
                program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
                program->use();
                s_uniformLocations[EUniforms::ColorTransformMult] =     glGetUniformLocation(program->getProgram(), s_uniformNames[EUniforms::ColorTransformMult]);
                s_uniformLocations[EUniforms::ColorTransformOffset] =   glGetUniformLocation(program->getProgram(), s_uniformNames[EUniforms::ColorTransformOffset]);
                s_uniformLocations[EUniforms::ColorMatrixBody] =        glGetUniformLocation(program->getProgram(), s_uniformNames[EUniforms::ColorMatrixBody]);
                s_uniformLocations[EUniforms::ColorMatrixAppendix] =    glGetUniformLocation(program->getProgram(), s_uniformNames[EUniforms::ColorMatrixAppendix]);

                CC_SAFE_RELEASE(s_programs[EPrograms::Alpha]);
                s_programs[EPrograms::Alpha] = program;
                CC_SAFE_RETAIN(s_programs[EPrograms::Alpha]);
            }

            // AlphaNoCtx
            {
                const char* fragmentShader = getShader(EFragmentShader::AlphaNoCtx);
                GLProgram* program = GLProgram::createWithByteArrays(cocos2d::ccPositionTextureColor_vert, fragmentShader);
                program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
                program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
                program->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
                program->use();
                s_uniformLocations[EUniforms::Alpha] = glGetUniformLocation(program->getProgram(), s_uniformNames[EUniforms::Alpha]);

                CC_SAFE_RELEASE(s_programs[EPrograms::AlphaNoCtx]);
                s_programs[EPrograms::AlphaNoCtx] = program;
                CC_SAFE_RETAIN(s_programs[EPrograms::AlphaNoCtx]);
            }

            s_initialized = true;
        }
    }
