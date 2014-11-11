const char* GlowFragmentShader_fs = STRINGIFY(

\n#ifdef GL_ES\n
	const lowp int GAUSSIAN_SAMPLES = 9;

	varying highp vec2 v_texCoord;
	varying highp vec2 v_step;
	uniform highp vec4 u_glowColor;
\n#else\n
	const int GAUSSIAN_SAMPLES = 9;

	varying vec2 v_texCoord;
	varying vec2 v_step;
	uniform vec4 u_glowColor;
\n#endif\n

void main()
{
\n#ifdef GL_ES\n
 	lowp vec4 sum = vec4(0.0);
\n#else\n
	vec4 sum = vec4(0.0);
\n#endif\n

    sum += texture2D(CC_Texture0, v_texCoord - v_step * 4.0).a * u_glowColor * 0.05;
    sum += texture2D(CC_Texture0, v_texCoord - v_step * 3.0).a * u_glowColor * 0.09;
    sum += texture2D(CC_Texture0, v_texCoord - v_step * 2.0).a * u_glowColor * 0.12;
    sum += texture2D(CC_Texture0, v_texCoord - v_step * 1.0).a * u_glowColor * 0.15;
    sum += texture2D(CC_Texture0, v_texCoord + v_step * 0.0).a * u_glowColor * 0.18;
    sum += texture2D(CC_Texture0, v_texCoord + v_step * 1.0).a * u_glowColor * 0.15;
    sum += texture2D(CC_Texture0, v_texCoord + v_step * 2.0).a * u_glowColor * 0.12;
    sum += texture2D(CC_Texture0, v_texCoord + v_step * 3.0).a * u_glowColor * 0.09;
    sum += texture2D(CC_Texture0, v_texCoord + v_step * 4.0).a * u_glowColor * 0.05;
    
	/*/color.a = clamp(color.a * Scale.x, 0.0, 1.0);*/
	
 	gl_FragColor = sum;
}
);
