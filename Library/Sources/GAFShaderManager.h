#pragma once

NS_GAF_BEGIN

class GAFShaderManager
{
public:
    static void renderRecreate(cocos2d::EventCustom*);

    struct EFragmentShader {
        enum Name {
            GaussBlur = 0,
            Glow,
            Alpha,

            SIZE
        };
    };
    
    struct EPrograms {
        enum Name {
            Alpha = 0,
            Blur,
            Glow,

            SIZE
        };
    };

    struct EUniforms {
        enum Name {
            ColorTransformMult = 0,
            ColorTransformOffset,
            ColorMatrixBody,
            ColorMatrixAppendix,

            BlurTexelOffset,

            GlowTexelOffset,
            GlowColor,


            SIZE
        };
    };


    static inline const char* getShader(EFragmentShader::Name n) { return s_fragmentShaders[n]; }

    static inline cocos2d::GLProgram* getProgram(EPrograms::Name n) { return s_programs[n]; }
    static inline GLint getUniformLocation(EUniforms::Name n) { return s_uniformLocations[n]; }
    static inline const char* getUniformName(EUniforms::Name n) { return s_uniformNames[n]; }

    static void Initialize(bool force = false);

private:
    static const char* const s_fragmentShaders[EFragmentShader::SIZE];
    static cocos2d::GLProgram* s_programs[EPrograms::SIZE];
    static const char* const s_uniformNames[EUniforms::SIZE];
    static GLint s_uniformLocations[EUniforms::SIZE];

    static bool s_initialized;
}; // GAFShaderManager

NS_GAF_END
