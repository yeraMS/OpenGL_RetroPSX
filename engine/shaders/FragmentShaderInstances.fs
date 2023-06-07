#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

in vec3 fragPos;
in vec3 ourNormal;


const float u_fogDepthMax = 5.0f;
const float u_fogDepthMin = 0.0f;

struct Material {
    sampler2D diffuse;
    sampler2D  specular;
    float     shininess;
};

uniform Material material;

struct directionalLight {
    // vec3 position; // no longer necessary when using directional lights.
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform directionalLight dirLight;

uniform vec3 objectColor;

vec3 calcDirectionalLight (directionalLight dLight, vec3 normal, vec3 viewDir, Material material) 
{
    vec3 ambient = dLight.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(-dLight.direction); //las directionals solo tienen una direccion todo en el mmismo angulo
    float diff = max(dot(normal, lightDir), 0.0); // calculamos como esta direccion interactua con nuestra noraml
    vec3 diffuse = dLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dLight.specular * spec * vec3(texture(material.specular, TexCoords));

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
    vec3 ambient  = pLight.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = pLight.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = pLight.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


uniform PointLight pointLights[1];

uniform vec3 viewerPos;

void main()
{
    vec4 textColor = texture(material.diffuse, TexCoords);
    if(textColor.a<0.1){
        discard;
    }


    vec3 viewDir = normalize(viewerPos-fragPos);


    vec3 norm = normalize(ourNormal);
    vec3 lightadittion = calcDirectionalLight(dirLight, norm, viewDir, material);

    lightadittion += calcPointLight(pointLights[0], norm, fragPos, viewDir, material);

    float depth = abs(gl_FragCoord.z/gl_FragCoord.w);
    float v_fogDensity = 0.0 + clamp((u_fogDepthMax - depth) / (u_fogDepthMin - u_fogDepthMax), 0.0, 1.0);
    vec3 outputfinal = mix(lightadittion,objectColor,v_fogDensity);
    FragColor = vec4(outputfinal,1.0f);
}