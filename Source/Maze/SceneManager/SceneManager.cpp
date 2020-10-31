#include "SceneManager.h"

SceneManager::SceneManager(std::unique_ptr<Camera>&& camera) :
    m_camera {std::move(camera)}
{
    InitScene();
}

SceneManager::~SceneManager()
{
    for(const auto& object : m_objects)
        object->Destroy();
}

void SceneManager::InitScene()
{
	glEnable(GL_DEPTH_TEST);

	float floorVertices[12] = {
		 150.f,  -5.f, 150.f,
		 150.f, -5.f, -150.f,
		-150.f, -5.f, -150.f,
		-150.f, -5.f, 150.f
	};
	unsigned int floorIndices[6] = {
	   0, 1, 3,
	   1, 2, 3
	};

    auto shader = CreateShader("Source/Maze/Shaders/vert.vs", "Source/Maze/Shaders/frag.fs");
    
    AddObject(std::make_unique<Object>("floor", glm::vec3{0.2f, 1.0f, 0.2f}, std::move(shader), floorVertices, floorIndices, 6));
}

void SceneManager::DrawScene()
{
    m_camera->Update();
    
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(const auto& object : m_objects)
        object->Draw();
}

void SceneManager::AddObject(std::unique_ptr<Object>&& object) noexcept
{
    m_objects.emplace_back(std::move(object));
}

std::unique_ptr<Shader> SceneManager::CreateShader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) const noexcept
{
    return std::make_unique<Shader>(*m_camera, vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());
}
