#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "SceneManager/SceneManager.h"
#include "Camera/FPSCamera.h"
#include "MapLoader/MapLoader.h"

#include <memory>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}


int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -2;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Maze", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -2;
	}
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	//----------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
    
    SceneManager sceneManager { std::make_unique<FPSCamera>(window) };
    
    MapLoader mapLoader{sceneManager};
    mapLoader.LoadFromBitmap("Data/maze.bmp", glm::vec3{-50.0f, -5.1f, -50.0f}, glm::vec2{2.0f, 2.0f});
    
    while (!glfwWindowShouldClose(window)) {
        sceneManager.SceneManager::DrawScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


