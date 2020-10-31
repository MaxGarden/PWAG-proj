#define STB_IMAGE_IMPLEMENTATION

#include "MapLoader.h"
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

std::unique_ptr<Object> MapLoader::CreateObject(unsigned char type, const glm::vec3& position, const glm::vec2& size) const noexcept
{
    switch(type)
    {
        case 0:
            return CreateWall(position, size);
        default:
            return nullptr;
    }
}

std::unique_ptr<Object> MapLoader::CreateWall(const glm::vec3& position, const glm::vec2& size) const noexcept
{
    static const auto wallHeight = 8.0f;
    
    float vertices[] =
    {
        position.x,  position.y, position.z,                                // 0 - LeftDown Bottom
        position.x + size.x,  position.y, position.z,                       // 1 - RightDown Bottom
        position.x + size.x,  position.y, position.z + size.y,              // 2 - RightUp Bottom
        position.x,  position.y, position.z + size.y,                       // 3 - LeftUp Bottom
        
        position.x,  position.y + wallHeight, position.z,                   // 4 - LeftDown Top
        position.x + size.x,  position.y + wallHeight, position.z,          // 5 - RightDown Top
        position.x + size.x,  position.y + wallHeight, position.z + size.y, // 6 - RightUp Top
        position.x,  position.y + wallHeight, position.z + size.y,          // 7 - LeftUp Top
    };
    
    // LeftDown Bottom, LeftUp Bottom, RightUp Bottom
    // LeftDown Bottom, RightUp Bottom, RightDown Bottom
    
    // LeftDown Bottom, LeftDown Top, LeftUp Bottom
    // LeftDown Bottom, LeftUp Bottom, LeftUp Top
    
    // LeftDown Top, LeftUp Top, RightUp Top
    // LeftDown Top, RightUp Top, RightDown Top
    
    // RightDown Bottom, RightDown Top, RightUp Top
    // RightDown Bottom, RightUp Top, RightUp Bottom
    
    // LeftDown Bottom, LeftDown Top, RightDown Top
    // LeftDown Bottom, RightDown Top, RightDown Bottom
    
    // LeftUp Bottom, LeftUp Top, RightUp Top
    // LeftUp Bottom, RightUp Top, RightUp Bottom
    
    unsigned int indices[] =
    {
        0, 3, 2,
        0, 2, 1,

        0, 4, 3,
        0, 3, 7,

        4, 7, 6,
        4, 6, 5,

        1, 5, 6,
        1, 6, 2,

        0, 4, 5,
        0, 5, 1,

        3, 7, 6,
        3, 6, 2,
    };

    auto shader = m_sceneManager.CreateShader("Source/Maze/Shaders/vert.vs", "Source/Maze/Shaders/frag.fs");
    
    return std::make_unique<Object>("wall", glm::vec3{0.3f, 0.4f, 1.0f}, std::move(shader), vertices, indices, 36);
}
