////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Gaussian blur
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* GaussianBlurFragmentShader_fs = STRINGIFY(

\n#ifdef GL_ES\n
    varying highp vec2 v_texCoord;
	uniform highp vec2 u_step;
\n#else\n
    varying vec2 v_texCoord;
	uniform vec2 u_step;
\n#endif\n

void main()
{
\n#ifdef GL_ES\n
    lowp vec4 sum = vec4(0.0);
\n#else\n
    vec4 sum = vec4(0.0);
\n#endif\n

    sum += texture2D(CC_Texture0, v_texCoord - u_step * 4.0) * 0.05;
    sum += texture2D(CC_Texture0, v_texCoord - u_step * 3.0) * 0.09;
    sum += texture2D(CC_Texture0, v_texCoord - u_step * 2.0) * 0.12;
    sum += texture2D(CC_Texture0, v_texCoord - u_step * 1.0) * 0.15;
    sum += texture2D(CC_Texture0, v_texCoord + u_step * 0.0) * 0.18;
    sum += texture2D(CC_Texture0, v_texCoord + u_step * 1.0) * 0.15;
    sum += texture2D(CC_Texture0, v_texCoord + u_step * 2.0) * 0.12;
    sum += texture2D(CC_Texture0, v_texCoord + u_step * 3.0) * 0.09;
    sum += texture2D(CC_Texture0, v_texCoord + u_step * 4.0) * 0.05;
    
    gl_FragColor = sum;
}

);