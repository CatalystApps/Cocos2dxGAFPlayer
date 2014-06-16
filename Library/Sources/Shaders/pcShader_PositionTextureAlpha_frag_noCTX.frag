////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple shader with added color transform
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* pcShader_PositionTextureAlpha_frag_noCTX_fs = STRINGIFY(

\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n

varying vec2 v_texCoord;
uniform float fragmentAlpha;

void main()
{

    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    texColor = vec4(texColor.x / texColor.a, texColor.y / texColor.a, texColor.z / texColor.a, texColor.a);
	texColor.a *= fragmentAlpha;
    texColor = vec4(texColor.x * texColor.a, texColor.y * texColor.a, texColor.z * texColor.a, texColor.a);
    
    gl_FragColor = texColor;

}
);
