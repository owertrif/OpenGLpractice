

#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;    
    float shininess;
}; 

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
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

#define POINT_LIGHTS_NUM 4

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform PointLight pointLights[POINT_LIGHTS_NUM];
uniform DirLight dirLight;

vec3 ComputePointLight(PointLight pointlight);
vec3 ComputeDirectLight(DirLight dirLight, vec3 norm, vec3 viewdir);

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
    
    for(int i = 0; i < POINT_LIGHTS_NUM; i++){
        result += ComputePointLight(pointLights[i]);
    }

    result += ComputeDirectLight(dirLight, norm, viewDir);

    FragColor = vec4(result, 1.0);
    
} 

vec3 ComputePointLight(PointLight pointlight){
    vec3 ambient = pointlight.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 lightDir = normalize(pointlight.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointlight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointlight.specular * spec * texture(material.specular,TexCoords).rgb;  
        
    float distance = length(pointlight.position - FragPos);
    float attenuation = 1.0/(pointlight.constant + pointlight.linear * distance + pointlight.quadratic * (distance*distance)); 

    ambient  *= attenuation;
    specular *= attenuation;
    diffuse  *= attenuation;

    ambient.x *= 0.0;
    diffuse.x *= 0.0;

    ambient.z *= 0.0;
    diffuse.z *= 0.0;

    vec3 result = ambient + diffuse +specular;

    return result;
}

vec3 ComputeDirectLight(DirLight dirLight, vec3 norm, vec3 viewdir){
    vec3 lightDir = normalize(-dirLight.direction);

    float diff = max(dot(norm,lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewdir, reflectDir),0.0),material.shininess);

    vec3 ambient = dirLight.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = dirLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = dirLight.specular * spec * texture(material.specular, TexCoords).rgb;

    return (ambient + diffuse + specular);
}