

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCoord;

const float u_fogDepthMax = 40.0f;
const float u_fogDepthMin = 10.0f;

uniform float posHor;
uniform mat4 transform;

out vec3 ourColor;
out vec2 TextCoord;
out vec3 v_fogDensity;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    ourColor = aColor;
    TextCoord = aTextCoord;
    gl_Position =  projection * view * model * vec4(aPos,1.0);
}

