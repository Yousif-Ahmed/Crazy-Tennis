#version 330 core
in  vec2  tex_coord;
out vec4  frag_color;
  
uniform sampler2D tex;
uniform vec2      offsets[9];
uniform int       edge_kernel[9];
uniform float     blur_kernel[9];


void main()
{
    frag_color = vec4(0.0f);
    vec3 sample[9];
    // sample from texture offsets if using convolution matrix
  
    for(int i = 0; i < 9; i++) 
    {
        sample[i] = vec3(texture(tex, tex_coord.st + offsets[i]));
        frag_color += vec4(sample[i] * blur_kernel[i], 0.0f);
    }
                
    frag_color.a = 1.0f;
}