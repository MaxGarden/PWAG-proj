#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../Object.h"
#include "../SceneManager/SceneManager.h"
#include "../Shader.h"

class MapLoader final
{
public:
    MapLoader(SceneManager& sceneManager);
    ~MapLoader() = default;
    
    bool LoadFromBitmap(const std::string& fileName, const glm::vec3& startOffset, const glm::vec2& cellSize) const noexcept;
    
private:
    bool LoadDataFromBitmap(const std::string& fileName, std::vector<unsigned char>& destination, size_t& width, size_t& height) const noexcept;
    
    Shader* GetShader() const noexcept;
    
    std::unique_ptr<Object> CreateObject(unsigned char type, const glm::vec3& position, const glm::vec2& size) const noexcept;
    
    std::unique_ptr<Object> CreateWall(const glm::vec3& position, const glm::vec2& size) const noexcept;
    std::unique_ptr<Object> CreateChest(const glm::vec3& position, const glm::vec2& size) const noexcept;
    std::unique_ptr<Object> CreateChestAquarium(const glm::vec3& position, const glm::vec2& size) const noexcept;
    
private:
    SceneManager& m_sceneManager;
};
