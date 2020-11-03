#include "FPSCamera.h"
#include <glm/gtc/matrix_transform.hpp>

static const glm::vec3 upDirection =
{
    0.0f,
    1.0f,
    0.0f
};

FPSCamera::FPSCamera(GLFWwindow* window, float fov) :
    Camera(window),
    m_fov {fov}
{
}

const glm::mat4& FPSCamera::GetViewMatrix() const noexcept
{
    if(m_isViewMatrixDirty)
    {
        m_viewMatrix = CalculateViewMatrix();
        m_isViewMatrixDirty = false;
    }
    
    return m_viewMatrix;
}

const glm::mat4& FPSCamera::GetProjectionMatrix() const noexcept
{
    if(m_isProjectionMatrixDirty)
    {
        m_projectionMatrix = CalculateProjectionMatrix();
        m_isProjectionMatrixDirty = false;
    }
    
    return m_projectionMatrix;
}

glm::vec3 FPSCamera::CalculateFrontDirection() const noexcept
{
    return glm::normalize(glm::vec3
    {
      cos(m_rotation.x) * cos(m_rotation.y),
      sin(m_rotation.y),
      sin(m_rotation.x) * cos(m_rotation.y)
    });
}

void FPSCamera::SetPosition(const glm::vec3& position)
{
    m_position = position;
    m_isViewMatrixDirty = true;
}

const glm::vec3& FPSCamera::GetPosition() const noexcept
{
    return m_position;
}

void FPSCamera::SetRotation(const glm::vec2& rotation)
{
    m_rotation = rotation;
    m_isViewMatrixDirty = true;
    m_isFrontDirectionDirty = true;
}

const glm::vec2& FPSCamera::GetRotation() const noexcept
{
    return m_rotation;
}

const glm::vec3& FPSCamera::GetFrontDirection() const noexcept
{
    if(m_isFrontDirectionDirty)
    {
        m_frontDirection = CalculateFrontDirection();
        m_isFrontDirectionDirty = false;
    }
    
    return m_frontDirection;
}

glm::mat4 FPSCamera::CalculateViewMatrix() const noexcept
{
    return glm::lookAt(m_position, m_position + GetFrontDirection(), upDirection);
}

glm::mat4 FPSCamera::CalculateProjectionMatrix() const noexcept
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(GetWindow(), &windowWidth, &windowHeight);
    
    const auto aspectRatio = static_cast<float>(windowWidth) / windowHeight;
    return glm::perspective(m_fov, aspectRatio, 0.1f, 500.0f);
}
