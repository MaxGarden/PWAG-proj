#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

class Object
{
public:
	Object(const std::string& textureName, Shader& shader, const float* vertices, const unsigned int* indices , unsigned int verticesCount);
    
	void Destroy();
	void Draw();
    
private:
    const std::string m_textureName;
    
    Shader& m_shader;
    unsigned int VBO, VAO, EBO;
    
    const unsigned int m_verticesCount;
};

