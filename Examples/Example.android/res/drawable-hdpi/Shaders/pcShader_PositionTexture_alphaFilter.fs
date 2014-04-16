////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Shader for masks with stencil, transparent pixels of texture will be discarded
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GL_ES
precision lowp float;		
#endif						
                                                             
varying vec2 v_texCoord;	
uniform sampler2D u_texture;
                                                             
void main()					
{							
    vec4 color = texture2D(u_texture, v_texCoord);        
    if (color.a > 0.1)
    {                                                        
        gl_FragColor = color;
    }                                                        
    else                                                     
    {                                                        
        discard;                                             
    }                                                        
}


