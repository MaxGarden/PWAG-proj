#version 330 core

out vec4 out_color;
uniform vec3 color;

in vec3 FragmentPosition;
in vec3 Normal;
in vec2 TextureCoords;

uniform sampler2D fragmentTexture;

struct Light
{
    vec3 position;
    vec3 frontDirection;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
};

uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - FragmentPosition);
        
    float theta = dot(lightDir, normalize(-light.frontDirection));
    

    vec3 ambient = light.ambient * texture(fragmentTexture, TextureCoords).rgb;
    
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(fragmentTexture, TextureCoords).rgb;
    
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    
    vec3 result = ambient + diffuse;
    out_color = vec4(result, 1.0);
}
