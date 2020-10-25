#pragma once

#include "Camera.h"

class FPSCamera final : public Camera
{
public:
    FPSCamera(GLFWwindow* window, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float fov = 45.0f, float moveSpeed = 10.0f, float lookSpeed = 0.005f);
    virtual ~FPSCamera() override = default;
    
    virtual const glm::mat4& GetViewMatrix() const noexcept override;
    virtual const glm::mat4& GetProjectionMatrix() const noexcept override;
    
protected:
    virtual void Update(float deltaTime) override;
    
private:
    glm::mat4 CalculateViewMatrix() const noexcept;
    glm::mat4 CalculateProjectionMatrix() const noexcept;
    
    void HandleInput(float deltaTime);
    
    void HandleMouse(float deltaTime);
    void HandleKeyboard(float deltaTime);
    
private:
    mutable bool m_isViewMatrixDirty = true;
    mutable bool m_isProjectionMatrixDirty = true;
    
    mutable glm::mat4 m_viewMatrix;
    mutable glm::mat4 m_projectionMatrix;
    
    glm::vec3 m_position;
    
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    
    bool m_initialized = false;
    
    const float m_fov;
    const float m_moveSpeed;
    const float m_lookSpeed;
    
    const glm::vec3 m_initialPosition;
};
