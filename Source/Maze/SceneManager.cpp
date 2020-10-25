#include "SceneManager.h"

SceneManager::SceneManager(std::unique_ptr<Camera>&& camera) :
    m_camera {std::move(camera)}
{
    InitScene();
    SetLight();
    SetFloor();
}

SceneManager::~SceneManager()
{
    object.Destroy();
}

void SceneManager::InitScene()
{
	glEnable(GL_DEPTH_TEST);

	float floorVertices[12] = {
		 50.f,  -5.f, 50.f,
		 50.f, -5.f, -50.f,
		-50.f, -5.f, -50.f,
		-50.f, -5.f, 50.f
	};
	unsigned int floorIndices[6] = {
	   0, 1, 3,
	   1, 2, 3
	};

	object = Object("floor", std::unique_ptr<Shader>(new Shader(*m_camera, "vert.vs", "frag.fs")), floorVertices, floorIndices, 6);
}

void SceneManager::DrawScene()
{
    m_camera->Update();
    
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	object.Draw();
}

void SceneManager::SetLight()
{
	/*glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);*/

}

void SceneManager::SetFloor()
{
	//Shader shader("vert.vs", "frag.fs");
	////SetMVP();
	//if (floorShaderHandle == 0)
	//{
	//	error = true;
	//	return;
	//}

	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	//glUseProgram(floorShaderHandle);
}
