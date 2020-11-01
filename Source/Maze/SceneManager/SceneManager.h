#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shader.h"
#include "../Object.h"
#include "../Camera/Camera.h"

class SceneManager
{
public:

	SceneManager(std::unique_ptr<Camera>&& camera);
	~SceneManager();
    
	void DrawScene();
    
    void AddObject(std::unique_ptr<Object>&& object) noexcept;
    
    Shader* EnsureShader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) const noexcept;

private:
    void InitScene();
    
private:
    std::unique_ptr<Camera> m_camera;
    std::vector<std::unique_ptr<Object>> m_objects;
    
    mutable std::map<std::tuple<std::string, std::string>, std::unique_ptr<Shader>> m_shadersMap;
};

