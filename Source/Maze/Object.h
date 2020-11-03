#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

class Object
{
public:
	Object(const std::string& textureName, const Shader& shader, const float* vertices, const unsigned int* indices , unsigned int verticesCount);
    
    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetPosition() const noexcept;
    
    void SetRotation(const glm::vec3& rotationInDegrees);
    const glm::vec3& GetRotation() const noexcept;
    
    void SetScale(const glm::vec3& scale);
    const glm::vec3& GetScale() const noexcept;
    
    bool IsCollidingWithSphere(const glm::vec3& center, float radius) const noexcept;
    
	void Destroy();
	void Draw();
    
private:
    glm::mat4 CalculateModelMatrix() const noexcept;
    const glm::mat4& GetModelMatrix() const noexcept;
    
    void MarkModelMatrixAsDirty() const noexcept;
    
    void IntializeBoxCollider(const float* vertices, size_t size) noexcept;
    
    void RecalculateBoxColliderIfNeeded() const noexcept;
    
private:
    const std::string m_textureName;
    const unsigned int m_verticesCount;
    const Shader& m_shader;
    
    glm::vec4 m_initialBoxColliderMin;
    glm::vec4 m_initialBoxColliderMax;
    
    glm::vec3 m_position = glm::vec3{0.0f, 0.0f, 0.0f};
    glm::vec3 m_rotationInDegrees = glm::vec3{0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale = glm::vec3{1.0f, 1.0f, 1.0f};
    
    mutable bool m_isModelMatrixDirty = true;
    mutable glm::mat4 m_modelMatrix;
    
    mutable bool m_isBoxColliderDirty = true;
    mutable glm::vec3 m_boxColliderMin;
    mutable glm::vec3 m_boxColliderMax;
    
    unsigned int VBO, VAO, EBO;
};

