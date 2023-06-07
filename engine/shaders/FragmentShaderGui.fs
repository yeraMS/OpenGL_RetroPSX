#version 330 core


out vec4 FragColor;
in vec2 TextCoord;

struct Material {
    sampler2D diffuse;
}; 



uniform Material material;

void main()
{
    vec4 texColor = texture(material.diffuse, TextCoord);
    if(texColor.a<0.1){
        discard;
    }
    FragColor = texColor;


}

