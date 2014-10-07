#pragma once

namespace gaf
{

class GAFShaderManager
{
public:
    static void handleEnterBackground();

    struct EFragmentShader {
    enum Name {
            GaussBlur,
            Glow,
            Masked,
            AlphaFilter,
            Alpha,
            AlphaNoCtx,

            SIZE
    };  };
    

    struct EVertexShader {
    enum Name {
            GaussBlur,

            SIZE
    };  };

    static inline const char* getShader(EFragmentShader::Name n) { return s_fragmentShaders[n]; }
    static inline const char* getShader(EVertexShader::Name n) { return s_vertexShaders[n]; }

private:
    static const char* const s_fragmentShaders[EFragmentShader::SIZE];
    static const char* const s_vertexShaders[EVertexShader::SIZE];

}; // GAFShaderManager

}