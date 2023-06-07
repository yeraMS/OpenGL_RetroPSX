

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTextCoord;

out vec2 TextCoord;

uniform mat4 model;

void main()
{
    TextCoord = aTextCoord;
    vec3 fragPos = vec3(model * vec4(aPos,1.0));
    fragPos.z = 0.0;
    gl_Position =  vec4(fragPos, 1.0);
}

