#include "ObjLoader.h"

ModelData::ModelData() :
    IsValid{false}
{
}

ModelData::ModelData(std::vector<float>&& vertices, std::vector<unsigned int>&& indices) :
    IsValid{true},
    Vertices{std::move(vertices)},
    Indices{std::move(indices)}
{
}


ObjLoader& ObjLoader::GetInstance() noexcept
{
    static ObjLoader objLoader;
    return objLoader;
}

const ModelData& ObjLoader::EnsureModelData(const std::string& fileName)
{
    static const ModelData null;
    
    const auto iterator = m_modelDataByFileName.find(fileName);
    if(iterator != std::cend(m_modelDataByFileName))
        return iterator->second ? *iterator->second : null;
    
    auto modelData = LoadModel(fileName);
    m_modelDataByFileName.emplace(fileName, std::move(modelData));
    return EnsureModelData(fileName);
}

std::unique_ptr<ModelData> ObjLoader::LoadModel(const std::string fileName) const
{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    const auto file = fopen(fileName.c_str(), "r");
    if(!file)
        return nullptr;

    while(true)
    {
        char lineHeader[128];
        
        const auto res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        
        if (strcmp( lineHeader, "v" ) == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
        else if (strcmp( lineHeader, "vt" ) == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            uv.y = -uv.y;
            temp_uvs.push_back(uv);
        }
        else if (strcmp( lineHeader, "vn" ) == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }
        else if (strcmp( lineHeader, "f" ) == 0)
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
            const auto matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
            if (matches != 12)
                return nullptr;
            
            vertexIndices.emplace_back(vertexIndex[0]);
            vertexIndices.emplace_back(vertexIndex[1]);
            vertexIndices.emplace_back(vertexIndex[2]);
            uvIndices    .emplace_back(uvIndex[0]);
            uvIndices    .emplace_back(uvIndex[1]);
            uvIndices    .emplace_back(uvIndex[2]);
            normalIndices.emplace_back(normalIndex[0]);
            normalIndices.emplace_back(normalIndex[1]);
            normalIndices.emplace_back(normalIndex[2]);
            
            vertexIndices.emplace_back(vertexIndex[0]);
            vertexIndices.emplace_back(vertexIndex[2]);
            vertexIndices.emplace_back(vertexIndex[3]);
            uvIndices    .emplace_back(uvIndex[0]);
            uvIndices    .emplace_back(uvIndex[2]);
            uvIndices    .emplace_back(uvIndex[3]);
            normalIndices.emplace_back(normalIndex[0]);
            normalIndices.emplace_back(normalIndex[2]);
            normalIndices.emplace_back(normalIndex[3]);
        }
        else
        {
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    std::vector<float> vertices;
    vertices.reserve(vertexIndices.size() * 5);
    
    for( unsigned int i=0; i<vertexIndices.size(); i++ )
    {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];
        
        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
        glm::vec2 uv = temp_uvs[ uvIndex-1 ];
        glm::vec3 normal = temp_normals[ normalIndex-1 ];
        
        vertices.emplace_back(vertex.x);
        vertices.emplace_back(vertex.y);
        vertices.emplace_back(vertex.z);
        
        vertices.emplace_back(normal.x);
        vertices.emplace_back(normal.y);
        vertices.emplace_back(normal.z);
        
        vertices.emplace_back(uv.x);
        vertices.emplace_back(uv.y);
    }
    
    std::vector<unsigned int> indices;
    indices.resize(vertexIndices.size());
    for(auto i = 0; i < indices.size(); ++i)
        indices[i] = i;
    
    fclose(file);
    return std::make_unique<ModelData>(std::move(vertices), std::move(indices));
}
