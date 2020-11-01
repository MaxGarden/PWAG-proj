#pragma once

#include <map>
#include <string>

class ResourcesManager final
{
public:
    static ResourcesManager& GetInstance() noexcept;
    
    unsigned int EnsureTextureId(const std::string& fileName);
    
private:
    ResourcesManager() = default;
    ~ResourcesManager() = default;
    
    unsigned int LoadTexture(const std::string fileName) const;
    
private:
    std::map<std::string, unsigned int> m_texturesIdByFileName;
};
