#version 330 core


out vec4 FragColor;

in vec3 ourNormal;
in vec2 TextCoord;
//in vec3 v_fogDensity;
in vec3 fragPos;

struct Material {
    sampler2D diffuse;
    sampler2D  specular;
    float     shininess;
}; 


// luz general, tipo sol
struct directionalLight {
    // vec3 position; // no longer necessary when using directional lights.
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


vec3 calcDirectionalLight (directionalLight dLight, vec3 normal, vec3 viewDir, Material material) 
{
    vec3 ambient = dLight.ambient * vec3(texture(material.diffuse, TextCoord));

    vec3 lightDir = normalize(-dLight.direction); //las directionals solo tienen una direccion todo en el mmismo angulo
    float diff = max(dot(normal, lightDir), 0.0); // calculamos como esta direccion interactua con nuestra noraml
    vec3 diffuse = dLight.diffuse * diff * vec3(texture(material.diffuse, TextCoord));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dLight.specular * spec * vec3(texture(material.specular, TextCoord));

    return (ambient + diffuse + specular);
}

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

vec3 calcPointLight(PointLight pLight, vec3 normal, vec3 fragPos, vec3 viewDir, Material material) 
{
    vec3 lightDir = normalize(pLight.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(pLight.position - fragPos);
    float attenuation = 1.0 / (pLight.constant + pLight.linear * distance + 
  			     pLight.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = pLight.ambient  * vec3(texture(material.diffuse, TextCoord));
    vec3 diffuse  = pLight.diffuse  * diff * vec3(texture(material.diffuse, TextCoord));
    vec3 specular = pLight.specular * spec * vec3(texture(material.specular, TextCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

/*
 We can set the material of the object in the application by setting the appropriate uniforms. 
 A struct in GLSL however is not special in any regard when setting uniforms; a struct only really acts as a namespace of uniform variables. 
 If we want to fill the struct we will have to set the individual uniforms, but prefixed with the struct's name: 
*/


uniform Material material;

uniform directionalLight dirLight;

#define NR_POINT_LIGHTS 15  
uniform int maxLights;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D OurTexture;
uniform sampler2D OurTexture2;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 objectColor;
uniform vec3 viewerPos;


const float u_fogDepthMax = 5.0f;
const float u_fogDepthMin = 0.0f;

void main()
{
    //FragColor = vec4(ourColor, 1.0f);
    //FragColor = texture(ourTexture,TextCoord)*vec4(ourColor, 1.0f);
    vec4 texColor = texture(material.diffuse, TextCoord);
    if(texColor.a<0.1){
        discard;
    }
    vec3 norm = normalize(ourNormal);
    vec3 viewDir = normalize(viewerPos-fragPos);

    vec3 lightadittion = calcDirectionalLight(dirLight, norm, viewDir, material);
    
    for (int i=0; i< maxLights; i++)
    {
        lightadittion += calcPointLight(pointLights[i], norm, fragPos, viewDir, material);
    }

    float depth = abs(gl_FragCoord.z/gl_FragCoord.w);
    float v_fogDensity = 0.0 + clamp((u_fogDepthMax - depth) / (u_fogDepthMin - u_fogDepthMax), 0.0, 1.0);
    vec3 outputfinal = mix(lightadittion,objectColor,v_fogDensity);
    FragColor = vec4(outputfinal,1.0f);
    //FragColor = mix(texture(OurTexture,TextCoord),texture(OurTexture2,TextCoord), 0.2);
    //hay que calcular la depth del FOG de alguna manera para pasarselo al vertex y que le meta el colorin a todo

    /*
        The final output color is now the combination of two texture lookups. GLSL's built-in mix function takes two values as input and linearly interpolates
        between them based on its third argument. If the third value is 0.0 it returns the first input; if it's 1.0 it returns the second input value. 
        A value of 0.2 will return 80% of the first input color and 20% of the second input color, resulting in a mixture of both our textures. 
    */
}

