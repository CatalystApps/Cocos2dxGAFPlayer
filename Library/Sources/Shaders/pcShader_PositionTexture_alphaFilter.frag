////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Shader for masks with stencil, transparent pixels of texture will be discarded
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* pcShader_PositionTexture_alphaFilter_fs = STRINGIFY(

\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n

varying vec2 v_texCoord;
uniform sampler2D u_texture;

void main()
{
    vec4 color = texture2D(u_texture, v_texCoord);
    if (color.a > 0.1)
    {
        gl_FragColor = color;
    }
    else
    {
        discard;
    }
}

);
