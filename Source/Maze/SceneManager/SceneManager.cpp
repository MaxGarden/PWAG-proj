#include "SceneManager.h"
#include <algorithm>

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

	float floorVertices[] =
    {
		 150.0f,     -5.0f,       150.0f,       0.0f,   1.0f,   0.0f, 1.0f, 1.0f,
		 150.0f,     -5.0f,       -150.0f,      0.0f,   1.0f,   0.0f, 1.0f, 0.0f,
		-150.0f,     -5.0f,       -150.0f,      0.0f,   1.0f,   0.0f, 0.0f, 0.0f,
		-150.0f,     -5.0f,       150.0f,       0.0f,   1.0f,   0.0f, 0.0f, 1.0f,
	};
    
	unsigned int floorIndices[] =
    {
	   0, 1, 3,
	   1, 2, 3
	};

    const auto shader = EnsureShader("Source/Maze/Shaders/vert.vs", "Source/Maze/Shaders/frag.fs");
    
    AddObject(std::make_unique<Object>("Data/Textures/grass.jpg", *shader, floorVertices, floorIndices, 6));
}

void SceneManager::DrawScene()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(const auto& object : m_objects)
        object->Draw();
}

void SceneManager::AddObject(std::unique_ptr<Object>&& object) noexcept
{
    m_objects.emplace_back(std::move(object));
}

void SceneManager::DestroyObject(const Object *object)
{
    const auto iterator = std::find_if(std::begin(m_objects), std::end(m_objects), [&object](const auto& objectUniquePtr)
    {
        return objectUniquePtr.get() == object;
    });
    
    if(iterator == std::cend(m_objects))
        return;
    
    (*iterator)->Destroy();
    m_objects.erase(iterator);
}

void SceneManager::VisitObjects(const std::function<bool(const Object&)>& visitor) const noexcept
{
    if(!visitor)
        return;
    
    for(const auto& object : m_objects)
    {
        if(!visitor(*object))
            break;
    }
}

Shader* SceneManager::EnsureShader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) const noexcept
{
    const auto shaderTuple = std::make_tuple(vertexShaderFileName, fragmentShaderFileName);
    const auto iterator = m_shadersMap.find(shaderTuple);
    if(iterator != std::cend(m_shadersMap))
        return iterator->second.get();
    
    auto shader = std::make_unique<Shader>(*m_camera, vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());
    
    auto x =std::pair<std::tuple<std::string, std::string>, std::unique_ptr<Shader>>(shaderTuple, nullptr);
    
    m_shadersMap.emplace(shaderTuple, std::move(shader));
    return EnsureShader(vertexShaderFileName, fragmentShaderFileName);
}

Camera& SceneManager::GetMainCamera()
{
    return *m_camera;
}
