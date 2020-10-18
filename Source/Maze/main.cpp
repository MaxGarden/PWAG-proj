#include <GLFW/glfw3.h>
#include <iostream>

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

    GLFWwindow* window = glfwCreateWindow(640, 480, "Maze", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -2;
    }
 
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
        glfwPollEvents();
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
