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
	char* ReadShader(char* aShaderFile);
	int SetVertexShader(char* vertexShaderFile);
	int SetFragmentShader(char* fragmentShaderFile);
	void SetShaders(char* vertexShaderFile, char* fragmentShaderFile);

public:
	int shaderProgramHandle;
	Shader(const Camera& camera, char* vertexShaderFile = nullptr, char* fragmentShaderFile = nullptr);
	~Shader();
	void setMat4(const char* name, const glm::mat4& mat);
	void SetVec3(const char* name, float x, float y, float z);
	void Use();
	void SetMVP();
    
private:
    const Camera& m_camera;

};

