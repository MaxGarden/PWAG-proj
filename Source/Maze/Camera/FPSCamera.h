#pragma once

#include "Camera.h"

class FPSCamera final : public Camera
{
public:
    FPSCamera(GLFWwindow* window, float fov = 45.0f);
    virtual ~FPSCamera() override = default;
    
    virtual const glm::mat4& GetViewMatrix() const noexcept override;
    virtual const glm::mat4& GetProjectionMatrix() const noexcept override;
    
    virtual void SetPosition(const glm::vec3& position) override;
    virtual const glm::vec3& GetPosition() const noexcept override;
    
    virtual void SetRotation(const glm::vec2& rotation) override;
    virtual const glm::vec2& GetRotation() const noexcept override;
    
    virtual void SetColorMultiplier(const glm::vec3& colorMultiplier) override;
    virtual const glm::vec3& GetColorMultiplier() const noexcept override;
    
    virtual const glm::vec3& GetFrontDirection() const noexcept override;
    
private:
    glm::mat4 CalculateViewMatrix() const noexcept;
    glm::mat4 CalculateProjectionMatrix() const noexcept;
    glm::vec3 CalculateFrontDirection() const noexcept;
    
private:
    const float m_fov;
    
    mutable bool m_isViewMatrixDirty = true;
    mutable bool m_isProjectionMatrixDirty = true;
    mutable bool m_isFrontDirectionDirty = true;
    
    mutable glm::mat4 m_viewMatrix;
    mutable glm::mat4 m_projectionMatrix;
    mutable glm::vec3 m_frontDirection;
    
    glm::vec3 m_position = glm::vec3{0.0f, 0.0f, 0.0f};
    glm::vec2 m_rotation = glm::vec2{0.0f, 0.0f};
    
    glm::vec3 m_colorMultiplier = glm::vec3{1.0f, 1.0f, 1.0f};
};
