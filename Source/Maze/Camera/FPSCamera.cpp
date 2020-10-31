#include "FPSCamera.h"

const glm::vec3 FPSCamera::s_upDirection =
{
    0.0f,
    1.0f,
    0.0f
};

FPSCamera::FPSCamera(GLFWwindow* window, glm::vec3 position, float fov, float moveSpeed, float lookSpeed) :
    Camera(window),
    m_position {position},
    m_fov {fov},
    m_moveSpeed {moveSpeed},
    m_lookSpeed {lookSpeed},
    m_initialPosition {position}
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

void FPSCamera::Update(float deltaTime)
{
    HandleInput(deltaTime);
    m_initialized = true;
}

void FPSCamera::HandleInput(float deltaTime)
{
    HandleMouse(deltaTime);
    HandleKeyboard(deltaTime);
}

void FPSCamera::HandleMouse(float deltaTime)
{
    static const auto epsilon = 0.0001f;
    
    const auto window = GetWindow();
    
    double cursorPositionX, cursorPositionY;
    glfwGetCursorPos(window, &cursorPositionX, &cursorPositionY);
    
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    const auto cursorCenterPositionX = windowWidth / 2.0;
    const auto cursorCenterPositionY = windowHeight / 2.0;
    glfwSetCursorPos(window, cursorCenterPositionX, cursorCenterPositionY);
    
    if(!m_initialized)
        return;
    
    const auto deltaX = cursorPositionX - cursorCenterPositionX;
    const auto deltaY = cursorPositionY - cursorCenterPositionY;
    
    if (abs(deltaX) > epsilon)
    {
        m_yaw += m_lookSpeed * deltaX;
        m_isViewMatrixDirty = true;
    }
    
    if (abs(deltaY) > epsilon)
    {
        m_pitch -= m_lookSpeed * deltaY;
        m_pitch = fmin(glm::radians(89.0f), fmax(glm::radians(-89.0f), m_pitch));
        m_isViewMatrixDirty = true;
    }
}

void FPSCamera::HandleKeyboard(float deltaTime)
{
    glm::vec3 cameraDirection
    {
        cos(m_yaw) * cos(m_pitch),
        sin(m_pitch),
        sin(m_yaw) * cos(m_pitch)
    };
    
    glm::vec3 cameraRight = glm::normalize(glm::cross(s_upDirection, cameraDirection));
    
    const auto window = GetWindow();
    const auto addToPosition = [this](const auto& delta)
    {
        m_position += delta;
        m_position.y = m_initialPosition.y;
        m_isViewMatrixDirty = true;
    };
    
    const auto isKeyPressed = [&window](int keyCode)
    {
        return glfwGetKey(window, keyCode) == GLFW_PRESS;
    };
    
    if (isKeyPressed(GLFW_KEY_UP) || isKeyPressed(GLFW_KEY_W))
        addToPosition(cameraDirection * deltaTime * m_moveSpeed);
    
    if (isKeyPressed(GLFW_KEY_DOWN) || isKeyPressed(GLFW_KEY_S))
        addToPosition(-cameraDirection * deltaTime * m_moveSpeed);
    
    if (isKeyPressed(GLFW_KEY_RIGHT) || isKeyPressed(GLFW_KEY_D))
        addToPosition(-cameraRight * deltaTime * m_moveSpeed);
    
    if (isKeyPressed(GLFW_KEY_LEFT) || isKeyPressed(GLFW_KEY_A))
        addToPosition(cameraRight * deltaTime * m_moveSpeed);
}

glm::mat4 FPSCamera::CalculateViewMatrix() const noexcept
{
    glm::vec3 direction
    {
        cos(m_yaw) * cos(m_pitch),
        sin(m_pitch),
        sin(m_yaw) * cos(m_pitch)
    };
    
    return glm::lookAt(m_position, m_position + glm::normalize(direction), s_upDirection);
}

glm::mat4 FPSCamera::CalculateProjectionMatrix() const noexcept
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(GetWindow(), &windowWidth, &windowHeight);
    
    const auto aspectRatio = static_cast<float>(windowWidth) / windowHeight;
    return glm::perspective(m_fov, aspectRatio, 0.1f, 500.0f);
}
