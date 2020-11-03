#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera(GLFWwindow* window);
    virtual ~Camera() = default;
    
    virtual const glm::mat4& GetViewMatrix() const noexcept = 0;
    virtual const glm::mat4& GetProjectionMatrix() const noexcept = 0;
    
    virtual const glm::vec3& GetFrontDirection() const noexcept = 0;
    
    virtual void SetPosition(const glm::vec3& position) = 0;
    virtual const glm::vec3& GetPosition() const noexcept = 0;
    
    virtual void SetRotation(const glm::vec2& rotation) = 0;
    virtual const glm::vec2& GetRotation() const noexcept = 0;
    
    GLFWwindow* GetWindow() const noexcept;
    
private:
    GLFWwindow* m_window;
};
