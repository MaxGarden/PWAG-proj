#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera/Camera.h"

class Shader
{
public:
	Shader(const Camera& camera, const char* vertexShaderFile = nullptr, const char* fragmentShaderFile = nullptr);
	~Shader();
    
    void SetColor(const glm::vec3& color) const noexcept;
    
    void Use();
    void Update();
    
private:
    char* ReadShader(const char* aShaderFile);
    int SetVertexShader(const char* vertexShaderFile);
    int SetFragmentShader(const char* fragmentShaderFile);
    void SetShaders(const char* vertexShaderFile, const char* fragmentShaderFile);
    
    void SetMat4(const char* name, const glm::mat4& value) const noexcept;
    void SetVec3(const char* name, const glm::vec3& value) const noexcept;
    void SetFloat(const char* name, float value) const noexcept;
    
private:
    const Camera& m_camera;
    int m_shaderProgramHandle;
};

