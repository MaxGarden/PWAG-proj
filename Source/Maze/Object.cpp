#include "Object.h"


Object::Object()
{
}

Object::Object(char* name, std::unique_ptr<Shader>&& sha, float* vert, unsigned int* indi, int numVer) :
    shader {std::move(sha)}
{
	objectName = name;
	vertices = vert;
	indices = indi;
	numVertices = numVer;
    
	shader->SetMVP();
	shader->SetVec3("color", 0.67f, 0.84f, 0.90f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * numVertices, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * numVertices, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
    shader->SetMVP();
    
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
}
