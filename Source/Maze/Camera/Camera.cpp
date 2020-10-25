#include "Camera.h"

Camera::Camera(GLFWwindow* window) :
    m_window {window},
    m_lastTime {glfwGetTime()}
{
}

void Camera::Update()
{
    const auto currentTime = glfwGetTime();
    const auto deltaTime = static_cast<float>(currentTime - m_lastTime);
    
    Update(deltaTime);
    
    m_lastTime = glfwGetTime();
}

GLFWwindow* Camera::GetWindow() const noexcept
{
    return m_window;
}
