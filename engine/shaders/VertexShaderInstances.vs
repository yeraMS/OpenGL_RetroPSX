#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

out vec3 ourNormal;

out vec3 fragPos;

void main()
{
    TexCoords = aTexCoords;

    ourNormal = aNormal;
    fragPos = vec3(aInstanceMatrix * vec4(aPos,1.0));
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f); 

    vec4 vert = gl_Position;
    vec4 snapped = vert;
	snapped.xyz = vert.xyz / vert.w;
	snapped.xy = floor(vec2(100,55) * snapped.xy) / vec2(100,55);
	snapped.xyz *= vert.w;

    gl_Position = snapped;
}