#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TextCoord;
in vec3 v_fogDensity;

uniform sampler2D OurTexture;
uniform sampler2D OurTexture2;

void main()
{
    FragColor = vec4(1.0f);

}