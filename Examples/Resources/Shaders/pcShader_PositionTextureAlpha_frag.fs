////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple shader with added color transform
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform vec4 colorTransform[2]; // mult + offsets
uniform mat4 colorMatrix;
uniform vec4 colorMatrix2;

uniform sampler2D u_texture;

void main()
{
    const float kMinimalAlphaAllowed = 0.001;

    vec4 texColor = texture2D(u_texture, v_texCoord);
    
    texColor.a = clamp(texColor.a, kMinimalAlphaAllowed, 1.0);   // to avoid division by 0
    texColor = vec4(texColor.rgb / texColor.a, texColor.a);

	vec4 ctxColor = texColor * colorTransform[0] + colorTransform[1];
	vec4 adjustColor = colorMatrix * ctxColor + colorMatrix2;
	
	texColor = vec4(adjustColor.rgb * adjustColor.a, adjustColor.a);
    
    gl_FragColor = texColor;
}
