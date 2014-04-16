uniform sampler2D inputImageTexture;

#ifdef GL_ES
	const lowp int GAUSSIAN_SAMPLES = 9;

	varying highp vec2 textureCoordinate;
	varying highp vec2 blurCoordinates[GAUSSIAN_SAMPLES];
	varying highp vec4 vGlowColor;
#else
	const int GAUSSIAN_SAMPLES = 9;

	varying vec2 textureCoordinate;
	varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];
	varying vec4 vGlowColor;
#endif

void main()
{
#ifdef GL_ES
 	lowp vec4 sum = vec4(0.0);
#else
	vec4 sum = vec4(0.0);
#endif

    sum += texture2D(inputImageTexture, blurCoordinates[0]).a * vGlowColor * 0.05;
    sum += texture2D(inputImageTexture, blurCoordinates[1]).a * vGlowColor * 0.09;
    sum += texture2D(inputImageTexture, blurCoordinates[2]).a * vGlowColor * 0.12;
    sum += texture2D(inputImageTexture, blurCoordinates[3]).a * vGlowColor * 0.15;
    sum += texture2D(inputImageTexture, blurCoordinates[4]).a * vGlowColor * 0.18;
    sum += texture2D(inputImageTexture, blurCoordinates[5]).a * vGlowColor * 0.15;
    sum += texture2D(inputImageTexture, blurCoordinates[6]).a * vGlowColor * 0.12;
    sum += texture2D(inputImageTexture, blurCoordinates[7]).a * vGlowColor * 0.09;
    sum += texture2D(inputImageTexture, blurCoordinates[8]).a * vGlowColor * 0.05;
    
	//color.a = clamp(color.a * Scale.x, 0.0, 1.0);
	
 	gl_FragColor = sum;
}

