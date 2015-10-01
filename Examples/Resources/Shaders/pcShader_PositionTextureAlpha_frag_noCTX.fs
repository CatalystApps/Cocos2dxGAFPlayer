////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple shader with added color transform
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float fragmentAlpha;

void main()
{
    const float kMinimalAlphaAllowed = 0.001;

    vec4 texColor = texture2D(u_texture, v_texCoord);
    
    texColor.a = clamp(texColor.a, kMinimalAlphaAllowed, 1.0);
    
    texColor = vec4(texColor.rgb / texColor.a, texColor.a);
	texColor.a *= fragmentAlpha;
    texColor = vec4(texColor.rgb * texColor.a, texColor.a);
    
    gl_FragColor = texColor;

}
