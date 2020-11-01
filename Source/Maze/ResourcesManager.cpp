#include "ResourcesManager.h"
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

ResourcesManager& ResourcesManager::GetInstance() noexcept
{
    static ResourcesManager resourcesManager;
    return resourcesManager;
}

unsigned int ResourcesManager::EnsureTextureId(const std::string& fileName)
{
    const auto iterator = m_texturesIdByFileName.find(fileName);
    if(iterator != std::cend(m_texturesIdByFileName))
        return iterator->second;
    
    const auto textureId = LoadTexture(fileName);
    m_texturesIdByFileName.emplace(fileName, textureId);
    return textureId;
}

unsigned int ResourcesManager::LoadTexture(const std::string fileName) const
{
    unsigned int result;
    glGenTextures(1, &result);

    int width, height, nrComponents;
    const auto data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, result);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    else
        std::cerr << "Texture failed to load at path: " << fileName << std::endl;

    stbi_image_free(data);
    return result;
}
