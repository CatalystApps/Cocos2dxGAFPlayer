////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple shader with added color transform
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform vec4 colorTransform[2]; // mult + offsets
uniform sampler2D u_texture;

void main()
{
    //const float kMinimalAlphaAllowed = 0.01;

    vec4 texColor = texture2D(u_texture, v_texCoord);
    
    //texColor.a = max(texColor.a, kMinimalAlphaAllowed);   // to avoid division by 0
    texColor = vec4(texColor.x / texColor.a, texColor.y / texColor.a, texColor.z / texColor.a, texColor.a);
	texColor = texColor * colorTransform[0] + colorTransform[1];
    texColor = vec4(texColor.x * texColor.a, texColor.y * texColor.a, texColor.z * texColor.a, texColor.a);
    
    gl_FragColor = texColor;
    
    //gl_FragColor = texture2D(u_texture, v_texCoord);
}
