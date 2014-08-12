const char* GaussianBlurVertexShader_vs = STRINGIFY(

attribute vec4 a_position;
attribute vec2 a_texCoord;

const int GAUSSIAN_SAMPLES = 9;

uniform float u_texelOffset_x;
uniform float u_texelOffset_y;

varying vec2 v_texCoord;
varying vec2 v_step;

void main()
{
 	gl_Position = CC_MVPMatrix * a_position;
 	v_texCoord = a_texCoord;
	 	    
 	// Calculate the positions for the blur
    v_step = vec2(u_texelOffset_x, u_texelOffset_y);
}
);