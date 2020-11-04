#define STB_IMAGE_IMPLEMENTATION

#include "MapLoader.h"
#include "../ObjLoader/ObjLoader.h"

#include <stb/stb_image.h>

MapLoader::MapLoader(SceneManager& sceneManager) :
    m_sceneManager{sceneManager}
{
}

bool MapLoader::LoadFromBitmap(const std::string& fileName, const glm::vec3& startOffset, const glm::vec2& cellSize) const noexcept
{
    int width, height, nrComponents;
    const auto data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
    if(!data)
        return false;
    
    auto position = startOffset;
    auto index = 0u;
    const auto size = width * height * nrComponents;
    for(auto i = 0; i < size; i+= nrComponents)
    {
        auto value = 0;
        for(auto j = 0; j < nrComponents; ++j)
            value += data[i + j];
        
        if(auto object = CreateObject(value / nrComponents, position, cellSize))
            m_sceneManager.AddObject(std::move(object));
        
        position.x += cellSize.x;
        if(++index % width == 0)
        {
            position.x = startOffset.x;
            position.z += cellSize.y;
        }
    }
    
    stbi_image_free(data);
    return true;
}

bool MapLoader::LoadDataFromBitmap(const std::string& fileName, std::vector<unsigned char>& destination, size_t& width, size_t& height) const noexcept
{
    const auto file = fopen(fileName.c_str(), "rb");
    if (!file)
        return false;

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, file);

    width = *(int*)&info[18];
    height = *(int*)&info[22];
    
    std::vector<unsigned char> data;
    data.resize((width * 3 + 3) & (~3));

    destination.clear();
    destination.reserve(width * height);
    
    for(auto i = 0u; i < height; i++)
    {
        fread(data.data(), sizeof(unsigned char), data.size(), file);
        for(auto j = 0u; j < width*3; j += 3)
        {
            const auto value = (static_cast<int>(data[j]) + static_cast<int>(data[j + 1]) + static_cast<int>(data[j + 2])) / 3;
            destination.emplace_back(static_cast<unsigned char>(value));
        }
    }
    
    fclose(file);
    return true;
}

Shader* MapLoader::GetShader() const noexcept
{
    return m_sceneManager.EnsureShader("Source/Maze/Shaders/vert.vs", "Source/Maze/Shaders/frag.fs");
}

std::unique_ptr<Object> MapLoader::CreateObject(unsigned char type, const glm::vec3& position, const glm::vec2& size) const noexcept
{
    switch(type)
    {
        case 0:
            return CreateWall(position, size);
        case 100:
            return CreateCoin(position, size);
        default:
            return nullptr;
    }
}

std::unique_ptr<Object> MapLoader::CreateWall(const glm::vec3& position, const glm::vec2& size) const noexcept
{
    static const auto wallHeight = 8.0f;
    const float vertices[] =
    {
        position.x,             position.y,                 position.z,             0.0f,   0.0f,   -1.0f,  0.0f,   0.0f,
        position.x + size.x,    position.y,                 position.z,             0.0f,   0.0f,   -1.0f,  1.0f,   0.0f,
        position.x + size.x,    position.y + wallHeight,    position.z,             0.0f,   0.0f,   -1.0f,  1.0f,   1.0f,
        position.x,             position.y + wallHeight,    position.z,             0.0f,   0.0f,   -1.0f,  0.0f,   1.0f,
    
        position.x,             position.y,                 position.z + size.y,    0.0f,   0.0f,   1.0f,   0.0f,   0.0f,
        position.x + size.x,    position.y,                 position.z + size.y,    0.0f,   0.0f,   1.0f,   1.0f,   0.0f,
        position.x + size.x,    position.y + wallHeight,    position.z + size.y,    0.0f,   0.0f,   1.0f,   1.0f,   1.0f,
        position.x,             position.y + wallHeight,    position.z + size.y,    0.0f,   0.0f,   1.0f,   0.0f,   1.0f,
        
        position.x,             position.y + wallHeight,    position.z + size.y,    -1.0f,  0.0f,   0.0f,   1.0f,   1.0f,
        position.x,             position.y + wallHeight,    position.z,             -1.0f,  0.0f,   0.0f,   0.0f,   1.0f,
        position.x,             position.y,                 position.z,             -1.0f,  0.0f,   0.0f,   0.0f,   0.0f,
        position.x,             position.y,                 position.z + size.y,    -1.0f,  0.0f,   0.0f,   1.0f,   0.0f,
        
        position.x + size.x,    position.y + wallHeight,    position.z + size.y,    1.0f,   0.0f,   0.0f,   1.0f,   1.0f,
        position.x + size.x,    position.y + wallHeight,    position.z,             1.0f,   0.0f,   0.0f,   0.0f,   1.0f,
        position.x + size.x,    position.y,                 position.z,             1.0f,   0.0f,   0.0f,   0.0f,   0.0f,
        position.x + size.x,    position.y,                 position.z + size.y,    1.0f,   0.0f,   0.0f,   1.0f,   0.0f,
        
        position.x,             position.y,                 position.z,             0.0f,   -1.0f,  0.0f,   0.0f,   0.0f,
        position.x + size.x,    position.y,                 position.z,             0.0f,   -1.0f,  0.0f,   1.0f,   0.0f,
        position.x + size.x,    position.y,                 position.z + size.y,    0.0f,   -1.0f,  0.0f,   1.0f,   1.0f,
        position.x,             position.y,                 position.z + size.y,    0.0f,   -1.0f,  0.0f,   0.0f,   1.0f,
        
        position.x,             position.y + wallHeight,    position.z,             0.0f,   1.0f,   0.0f,   0.0f,   0.0f,
        position.x + size.x,    position.y + wallHeight,    position.z,             0.0f,   1.0f,   0.0f,   1.0f,   0.0f,
        position.x + size.x,    position.y + wallHeight,    position.z + size.y,    0.0f,   1.0f,   0.0f,   1.0f,   1.0f,
        position.x,             position.y + wallHeight,    position.z + size.y,    0.0f,   1.0f,   0.0f,   0.0f,   1.0f,
    };
    
    const unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0,
        
        4, 5, 6,
        6, 7 ,4,
        
        8, 9, 10,
        10, 11, 8,
        
        12, 13, 14,
        14, 15, 12,
        
        16, 17, 18,
        18, 19, 16,
        
        20, 21, 22,
        22, 23, 20
    };

    auto result = std::make_unique<Object>(std::vector<std::string>{"Data/Textures/cobble.jpg"}, *GetShader(), vertices, indices, 36);
    result->SetFlags(Object::Colliding);
    
    return result;
}

std::unique_ptr<Object> MapLoader::CreateCoin(const glm::vec3& position, const glm::vec2& size) const noexcept
{
    const auto& modelData = ObjLoader::GetInstance().EnsureModelData("Data/Models/chest.obj");
    if(!modelData.IsValid)
        return nullptr;
    
    auto result = std::make_unique<Object>(std::vector<std::string>{"Data/Textures/chest.jpg"}, *GetShader(), modelData.Vertices.data(), modelData.Indices.data(), modelData.Indices.size());
    
    result->SetRotation(glm::vec3{-90.0f, 0.0f, 0.0f});
    result->SetScale(glm::vec3{0.03f, 0.03f, 0.03f});
    result->SetPosition(position);
    result->SetFlags(Object::Collectable);
    
    return result;
}
