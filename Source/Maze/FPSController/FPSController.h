#pragma once

#include "../SceneManager/SceneManager.h"

class FPSController final
{
public:
    FPSController(SceneManager& sceneManager, glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f}, float moveSpeed = 20.0f, float lookSpeed = 0.005);
    ~FPSController() = default;
    
    void Update();
    
private:
    void Update(float deltaTime);
    
    void HandleInput(float deltaTime);
    
    void HandleMouse(float deltaTime);
    void HandleKeyboard(float deltaTime);
    
    void RecalculateDirectionsIfNeeded() const noexcept;
    
    const glm::vec3& GetRightDirection() const noexcept;
    
    const glm::vec3& GetMoveFrontDirection() const noexcept;
    const glm::vec3& GetMoveRightDirection() const noexcept;
    
private:
    const glm::vec3 m_initialPosition;
    const float m_moveSpeed;
    const float m_lookSpeed;
    
    SceneManager& m_sceneManager;
    
    mutable bool m_isAnyDirectionDirty = true;
    
    mutable glm::vec3 m_rightDirection;
    mutable glm::vec3 m_moveFrontDirection;
    mutable glm::vec3 m_moveRightDirection;
    
    bool m_freeLook = false;
    
    double m_lastTime;
    bool m_initialized = false;
    
    GLenum m_polygonMode = GL_FILL;
};
