#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

struct ModelData
{
    const bool IsValid;
    
    const std::vector<float> Vertices;
    const std::vector<unsigned int> Indices;
    
    ModelData();
    ModelData(std::vector<float>&& vertices, std::vector<unsigned int>&& indices);
};

class ObjLoader final
{
public:
    static ObjLoader& GetInstance() noexcept;
    
    const ModelData& EnsureModelData(const std::string& fileName);
    
private:
    ObjLoader() = default;
    ~ObjLoader() = default;
    
    std::unique_ptr<ModelData> LoadModel(const std::string fileName) const;
    
private:
    std::map<std::string, std::unique_ptr<ModelData>> m_modelDataByFileName;
};
