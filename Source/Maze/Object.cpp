#include "Object.h"
#include "ResourcesManager.h"
#include <algorithm>

static const auto componentsCount = 8;

Object::Object(std::string&& textureName, const Shader& shader, const float* vertices, unsigned const int* indices, unsigned int verticesCount, std::string&& secondTextureName) :
    m_textureName{std::move(textureName)},
    m_verticesCount{verticesCount},
    m_shader{shader},
    m_secondTextureName{std::move(secondTextureName)}
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * componentsCount * m_verticesCount, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_verticesCount, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, componentsCount * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, componentsCount * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, componentsCount * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
	
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

    IntializeBoxCollider(vertices, verticesCount);
    
    m_material = Material
    {
        glm::vec3{0.2f, 0.2f, 0.2f},
        glm::vec3{0.2f, 0.2f, 0.2f},
        glm::vec3{0.0f, 0.0f, 0.0f},
        0.0f
    };
    
	m_shader.Use();
}

void Object::SetFlags(long flags)
{
    m_flags = flags;
}

long Object::GetFlags() const noexcept
{
    return m_flags;
}

void Object::SetPosition(const glm::vec3& position)
{
    m_position = position;
    MarkModelMatrixAsDirty();
}

const glm::vec3& Object::GetPosition() const noexcept
{
    return m_position;
}

void Object::SetRotation(const glm::vec3& rotationInDegrees)
{
    m_rotationInDegrees = rotationInDegrees;
    MarkModelMatrixAsDirty();
}

const glm::vec3& Object::GetRotation() const noexcept
{
    return m_rotationInDegrees;
}

void Object::SetScale(const glm::vec3& scale)
{
    m_scale = scale;
    MarkModelMatrixAsDirty();
}

const glm::vec3& Object::GetScale() const noexcept
{
    return m_scale;
}

void Object::SetMaterial(const Material& material)
{
    m_material = material;
}

const Material& Object::GetMaterial() const noexcept
{
    return m_material;
}

bool Object::IsCollidingWithSphere(const glm::vec3& center, float radius) const noexcept
{
    RecalculateBoxColliderIfNeeded();
    
    const glm::vec3 point
    {
        std::max(m_boxColliderMin.x, std::min(center.x, m_boxColliderMax.x)),
        std::max(m_boxColliderMin.y, std::min(center.y, m_boxColliderMax.y)),
        std::max(m_boxColliderMin.z, std::min(center.z, m_boxColliderMax.z))
    };

    return glm::distance(point, center) < radius;
}

void Object::Destroy()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Object::Draw()
{
    m_shader.Use();
    m_shader.Update();
    m_shader.SetColor(glm::vec3{1.0f, 1.0f, 1.0f});
    m_shader.SetModelMatrix(GetModelMatrix());
    m_shader.SetMaterial(GetMaterial());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ResourcesManager::GetInstance().EnsureTextureId(m_textureName));
    
    if(m_secondTextureName.length() > 0)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ResourcesManager::GetInstance().EnsureTextureId(m_secondTextureName));
        m_shader.SetSecondTexture(1);
    }
    
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, 0);
}

glm::mat4 Object::CalculateModelMatrix() const noexcept
{
    auto result = glm::mat4{1.0f};
    
    result = glm::translate(result, m_position);
    result = glm::rotate(result, glm::radians(m_rotationInDegrees.x), glm::vec3{1.0f, 0.0f, 0.0f});
    result = glm::rotate(result, glm::radians(m_rotationInDegrees.y), glm::vec3{0.0f, 1.0f, 0.0f});
    result = glm::rotate(result, glm::radians(m_rotationInDegrees.z), glm::vec3{0.0f, 0.0f, 1.0f});
    result = glm::scale(result, m_scale);
    
    return result;
}

const glm::mat4& Object::GetModelMatrix() const noexcept
{
    if(m_isModelMatrixDirty)
    {
        m_modelMatrix = CalculateModelMatrix();
        m_isModelMatrixDirty = false;
    }
    
    return m_modelMatrix;
}

void Object::MarkModelMatrixAsDirty() const noexcept
{
    m_isModelMatrixDirty = true;
    m_isBoxColliderDirty = true;
}

void Object::IntializeBoxCollider(const float* vertices, size_t size) noexcept
{
    m_initialBoxColliderMin = m_initialBoxColliderMax = glm::vec4
    {
        vertices[0],
        vertices[1],
        vertices[2],
        1.0f
    };
    
    for(auto i = componentsCount; i < size; i+=componentsCount)
    {
        const auto x = vertices[i];
        const auto y = vertices[i + 1];
        const auto z = vertices[i + 2];
        
        m_initialBoxColliderMin.x = std::min(m_initialBoxColliderMin.x, x);
        m_initialBoxColliderMin.y = std::min(m_initialBoxColliderMin.y, y);
        m_initialBoxColliderMin.z = std::min(m_initialBoxColliderMin.z, z);
        
        m_initialBoxColliderMax.x = std::max(m_initialBoxColliderMax.x, x);
        m_initialBoxColliderMax.y = std::max(m_initialBoxColliderMax.y, y);
        m_initialBoxColliderMax.z = std::max(m_initialBoxColliderMax.z, z);
    }
}

void Object::RecalculateBoxColliderIfNeeded() const noexcept
{
    if(!m_isBoxColliderDirty)
        return;
    
    const auto& modelMatrix = GetModelMatrix();
    
    m_boxColliderMin = modelMatrix * m_initialBoxColliderMin;
    m_boxColliderMax = modelMatrix * m_initialBoxColliderMax;
    
    m_isBoxColliderDirty = false;
}
