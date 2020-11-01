#include "Object.h"
#include "ResourcesManager.h"

Object::Object(const std::string& textureName, std::unique_ptr<Shader>&& sha, float* vert, unsigned int* indi, int numVer) :
    shader {std::move(sha)},
    m_textureName{textureName}
{
    static const auto numberOfComponents = 8;
    
	vertices = vert;
	indices = indi;
	numVertices = numVer;
    
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfComponents * numVertices, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numVertices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numberOfComponents * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, numberOfComponents * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, numberOfComponents * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
	
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	shader->Use();
}


void Object::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Object::Draw()
{
    shader->Use();
    shader->Update();
    shader->SetColor(glm::vec3{1.0f, 1.0f, 1.0f});
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ResourcesManager::GetInstance().EnsureTextureId(m_textureName));
    
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
}
