const char* GlowFragmentShader_fs = STRINGIFY(

\n#ifdef GL_ES\n
	const lowp int GAUSSIAN_SAMPLES = 9;

	varying highp vec2 v_texCoord;
	varying highp vec2 v_blurCoord[GAUSSIAN_SAMPLES];
	uniform highp vec4 u_glowColor;
\n#else\n
	const int GAUSSIAN_SAMPLES = 9;

	varying vec2 v_texCoord;
	varying vec2 v_blurCoord[GAUSSIAN_SAMPLES];
	uniform vec4 u_glowColor;
\n#endif\n

void main()
{
\n#ifdef GL_ES\n
 	lowp vec4 sum = vec4(0.0);
\n#else\n
	vec4 sum = vec4(0.0);
\n#endif\n

    sum += texture2D(CC_Texture0, v_blurCoord[0]).a * u_glowColor * 0.05;
    sum += texture2D(CC_Texture0, v_blurCoord[1]).a * u_glowColor * 0.09;
    sum += texture2D(CC_Texture0, v_blurCoord[2]).a * u_glowColor * 0.12;
    sum += texture2D(CC_Texture0, v_blurCoord[3]).a * u_glowColor * 0.15;
    sum += texture2D(CC_Texture0, v_blurCoord[4]).a * u_glowColor * 0.18;
    sum += texture2D(CC_Texture0, v_blurCoord[5]).a * u_glowColor * 0.15;
    sum += texture2D(CC_Texture0, v_blurCoord[6]).a * u_glowColor * 0.12;
    sum += texture2D(CC_Texture0, v_blurCoord[7]).a * u_glowColor * 0.09;
    sum += texture2D(CC_Texture0, v_blurCoord[8]).a * u_glowColor * 0.05;
    
	/*/color.a = clamp(color.a * Scale.x, 0.0, 1.0);*/
	
 	gl_FragColor = sum;
}
);
