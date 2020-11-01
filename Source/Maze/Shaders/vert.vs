#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTextureCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragmentPosition;
out vec3 Normal;
out vec2 TextureCoords;

void main()
{
    FragmentPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
    TextureCoords = vertexTextureCoords;
   
    gl_Position = projectionMatrix * viewMatrix * vec4(FragmentPosition, 1.0);
}
