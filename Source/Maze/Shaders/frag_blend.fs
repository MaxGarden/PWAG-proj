#version 330 core

out vec4 out_color;
uniform vec3 color;

in vec3 FragmentPosition;
in vec3 Normal;
in vec2 TextureCoords;

uniform sampler2D fragmentTexture;
uniform sampler2D secondTexture;

struct Light
{
    vec3 position;
    vec3 frontDirection;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 specularPosition;
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
  
uniform Light light;
uniform Material material;
uniform vec3 colorMultiplier;

void main()
{
    vec3 lightDir = normalize(light.position - FragmentPosition);
        
    float theta = dot(lightDir, normalize(-light.frontDirection));

    vec3 ambient = color * light.ambient * texture(fragmentTexture, TextureCoords).rgb * texture(secondTexture, TextureCoords).rgb * material.ambient;
    
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(fragmentTexture, TextureCoords).rgb * texture(secondTexture, TextureCoords).rgb * material.diffuse;
    
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    
    vec3 viewDir = normalize(light.specularPosition - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = color * texture(fragmentTexture, TextureCoords).rgb * texture(secondTexture, TextureCoords).rgb * (spec * material.specular);
    
    vec3 result = ambient + diffuse + specular;
    out_color = vec4(result * colorMultiplier, 1.0);
}
