////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple shader with added color transform
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float fragmentAlpha;

void main()
{

    vec4 texColor = texture2D(u_texture, v_texCoord);
    texColor = vec4(texColor.x / texColor.a, texColor.y / texColor.a, texColor.z / texColor.a, texColor.a);
	texColor.a *= fragmentAlpha;
    texColor = vec4(texColor.x * texColor.a, texColor.y * texColor.a, texColor.z * texColor.a, texColor.a);
    
    gl_FragColor = texColor;

}
