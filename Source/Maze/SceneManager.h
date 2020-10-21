#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Object.h"

class SceneManager
{
private:

	void SetLight();
	void SetFloor();
	void InitScene();

public:
	Object object; // do zmiany na liste, czy cos
	bool error = false;

	SceneManager();
	~SceneManager();
	void DrawScene();


};

