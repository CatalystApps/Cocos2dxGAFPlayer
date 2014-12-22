////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple shader with added color transform
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* pcShader_PositionTextureAlpha_frag_fs = STRINGIFY(

\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

uniform vec4 colorTransformMult;
uniform vec4 colorTransformOffsets;
uniform mat4 colorMatrix;
uniform vec4 colorMatrix2;

void main()
{
    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    
    const float kMinimalAlphaAllowed = 1.0e-8;

    if (texColor.a > kMinimalAlphaAllowed)
    {
        texColor = vec4(texColor.rgb / texColor.a, texColor.a);

        vec4 ctxColor = texColor * colorTransformMult + colorTransformOffsets;
        vec4 adjustColor = colorMatrix * ctxColor + colorMatrix2;
        adjustColor *= v_fragmentColor;

        texColor = vec4(adjustColor.rgb * adjustColor.a, adjustColor.a);
    }
    
    gl_FragColor = texColor;
}
);
