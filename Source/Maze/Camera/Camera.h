#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera(GLFWwindow* window);
    virtual ~Camera() = default;
    
    virtual const glm::mat4& GetViewMatrix() const noexcept = 0;
    virtual const glm::mat4& GetProjectionMatrix() const noexcept = 0;
    
    virtual const glm::vec3& GetPosition() const noexcept = 0;
    virtual const glm::vec3& GetFrontDirection() const noexcept = 0;
    
    void Update();
    
protected:
    GLFWwindow* GetWindow() const noexcept;
    
    virtual void Update(float deltaTime) = 0;
    
private:
    GLFWwindow* m_window;
    double m_lastTime;
};
