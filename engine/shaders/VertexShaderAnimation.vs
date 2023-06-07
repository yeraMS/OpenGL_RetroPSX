

#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;
layout (location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;


//uniform float posHor;
//uniform mat4 transform;

out vec3 ourNormal;
out vec2 TextCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];




void main()
{
    //gl_Position = transform * vec4(aPos.x+posHor,aPos.yz, 1.0);  rotar puede dar problemas si el shader tiene que cambiar la pos ya que el eje tmb rota
    
    //gl_Position = transform * vec4(aPos.x,aPos.yz, 1.0);
    ourNormal = aNormal;
    //ourNormal = mat3(transpose(inverse(model))) * aNormal; 
    TextCoord = aTextCoord;
    fragPos = vec3(model * vec4(aPos,1.0));
    //gl_Position =  projection * view *vec4(fragPos, 1.0);

    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
    }
		
    mat4 viewModel = view * model;
    gl_Position =  projection * viewModel * totalPosition;
    
    vec4 vert = gl_Position;
    vec4 snapped = vert;
	snapped.xyz = vert.xyz / vert.w;
	snapped.xy = floor(vec2(200,112) * snapped.xy) / vec2(200,112);
	snapped.xyz *= vert.w; 

    gl_Position = snapped;
    //gl_Position = snapped;
    //vec4 depthVert = vec4(model) + vec4(view); //modelo * vista para obtener depth
    //float depth = abs(depthVert.z / depthVert.w);
    //v_fogDensity.x = 1.0 - clamp((u_fogDepthMax - depth) / (u_fogDepthMin - u_fogDepthMax), 0.0, 1.0);
}

