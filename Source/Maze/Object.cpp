#include "Object.h"
#include "ResourcesManager.h"

Object::Object(const std::string& textureName, const Shader& shader, const float* vertices, unsigned const int* indices, unsigned int verticesCount) :
    m_textureName{textureName},
    m_verticesCount{verticesCount},
    m_shader{shader}
{
    static const auto componentsCount = 8;
    
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * componentsCount * m_verticesCount, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_verticesCount, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, componentsCount * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, componentsCount * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, componentsCount * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
	
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	m_shader.Use();
    RecalculateModelMatrix();
}

void Object::SetPosition(const glm::vec3& position)
{
    m_position = position;
    RecalculateModelMatrix();
}

const glm::vec3& Object::GetPosition() const noexcept
{
    return m_position;
}

void Object::SetRotation(const glm::vec3& rotationInDegrees)
{
    m_rotationInDegrees = rotationInDegrees;
    RecalculateModelMatrix();
}

const glm::vec3& Object::GetRotation() const noexcept
{
    return m_rotationInDegrees;
}

void Object::SetScale(const glm::vec3& scale)
{
    m_scale = scale;
    RecalculateModelMatrix();
}

const glm::vec3& Object::GetScale() const noexcept
{
    return m_scale;
}

void Object::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Object::Draw()
{
    m_shader.Use();
    m_shader.Update();
    m_shader.SetColor(glm::vec3{1.0f, 1.0f, 1.0f});
    m_shader.SetModelMatrix(m_modelMatrix);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ResourcesManager::GetInstance().EnsureTextureId(m_textureName));
    
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, 0);
}

void Object::RecalculateModelMatrix()
{
    m_modelMatrix = glm::mat4{1.0f};
    
    m_modelMatrix = glm::translate(m_modelMatrix, m_position);
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotationInDegrees.x), glm::vec3{1.0f, 0.0f, 0.0f});
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotationInDegrees.y), glm::vec3{0.0f, 1.0f, 0.0f});
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotationInDegrees.z), glm::vec3{0.0f, 0.0f, 1.0f});
    m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}

