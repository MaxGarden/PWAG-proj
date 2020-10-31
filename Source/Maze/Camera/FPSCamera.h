#pragma once

#include "Camera.h"

class FPSCamera final : public Camera
{
private:
    static const glm::vec3 s_upDirection;
    
public:
    FPSCamera(GLFWwindow* window, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float fov = 45.0f, float moveSpeed = 20.0f, float lookSpeed = 0.005f);
    virtual ~FPSCamera() override = default;
    
    virtual const glm::mat4& GetViewMatrix() const noexcept override;
    virtual const glm::mat4& GetProjectionMatrix() const noexcept override;
    
    virtual const glm::vec3& GetPosition() const noexcept override;
    virtual const glm::vec3& GetFrontDirection() const noexcept override;
    
protected:
    virtual void Update(float deltaTime) override;
    
private:
    glm::mat4 CalculateViewMatrix() const noexcept;
    glm::mat4 CalculateProjectionMatrix() const noexcept;
    glm::vec3 CalculateFrontDirection() const noexcept;
    
    void HandleInput(float deltaTime);
    
    void HandleMouse(float deltaTime);
    void HandleKeyboard(float deltaTime);
    
private:
    mutable bool m_isViewMatrixDirty = true;
    mutable bool m_isProjectionMatrixDirty = true;
    mutable bool m_isFrontDirectionDirty = true;
    
    mutable glm::mat4 m_viewMatrix;
    mutable glm::mat4 m_projectionMatrix;
    mutable glm::vec3 m_frontDirection;
    
    glm::vec3 m_position;
    
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    
    bool m_initialized = false;
    
    bool m_freeLook = false;
    
    const float m_fov;
    const float m_moveSpeed;
    const float m_lookSpeed;
    
    const glm::vec3 m_initialPosition;
};
