

#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;    
    float shininess;
}; 

struct SpotLight{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

#define SPOT_LIGHTS_NUM 4

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform SpotLight spotLights[SPOT_LIGHTS_NUM];

vec3 ComputeSpotLight(SpotLight spotlight);

void main()
{    
    vec3 lightDir = normalize(light.position - FragPos);

  	float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular,TexCoords).rgb;  
        
    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance*distance));

    specular *= intensity;
    diffuse  *= intensity;

    ambient  *= attenuation;
    specular *= attenuation;
    diffuse  *= attenuation;
    
    vec3 result = ambient + diffuse +specular;
    
    for(int i = 0; i < SPOT_LIGHTS_NUM; i++){
        result += ComputeSpotLight(spotLights[i]);
    }

    FragColor = vec4(result, 1.0);
    
} 

vec3 ComputeSpotLight(SpotLight spotlight){
    vec3 ambient = spotlight.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 lightDir = normalize(spotlight.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotlight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotlight.specular * spec * texture(material.specular,TexCoords).rgb;  
        
    float distance = length(spotlight.position - FragPos);
    float attenuation = 1.0/(spotlight.constant + spotlight.linear * distance + spotlight.quadratic * (distance*distance));

    ambient  *= attenuation;
    specular *= attenuation;
    diffuse  *= attenuation;

    vec3 result = ambient + diffuse +specular;

    return result;
}