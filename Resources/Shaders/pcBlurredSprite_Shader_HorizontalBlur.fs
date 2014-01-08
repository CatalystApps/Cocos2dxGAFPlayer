#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D u_texture;
varying vec2 v_texCoord;
uniform int u_matrixRowSize; // should be an odd number >= 1
uniform float dotSize; // specifies distance between color picks (can be 1/texSize which equals one pixel per step)
uniform float u_matrixRowValues[51];

void main()
{
    int i;
    vec4 sum = vec4(0.0);
    int halfSize = (u_matrixRowSize - 1) / 2;
	
    for (i = 0; i < u_matrixRowSize; i ++)
    {
        sum += texture2D(u_texture, vec2(v_texCoord.x + dotSize * float(i - halfSize), v_texCoord.y)) * u_matrixRowValues[i];
    }
	
    // applying premultiplies alpha
    gl_FragColor = vec4(sum.x * sum.a, sum.y * sum.a, sum.z * sum.a, sum.a);
}


