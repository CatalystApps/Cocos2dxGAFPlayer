////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple shader with added color transform
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* pcShader_PositionTextureAlpha_frag_noCTX_fs = STRINGIFY(

\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;
uniform float fragmentAlpha;

void main()
{
    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    texColor *= fragmentAlpha * v_fragmentColor;
    
    gl_FragColor = texColor;
}
);
