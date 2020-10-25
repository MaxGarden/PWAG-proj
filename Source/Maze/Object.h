#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"

#include <memory>

class Object
{
	char* objectName;
    std::unique_ptr<Shader> shader;
	float * vertices;
	unsigned int numVertices;
	unsigned int* indices;
	unsigned int VBO, VAO, EBO;

public:
	Object();
	Object(char* name, std::unique_ptr<Shader>&& shader, float* vertices, unsigned int* indices , int numVer);
	void Destroy();
	void Draw();
};

