#include "Camera.h"

Camera::Camera(GLFWwindow* window) :
    m_window {window}
{
}

GLFWwindow* Camera::GetWindow() const noexcept
{
    return m_window;
}
