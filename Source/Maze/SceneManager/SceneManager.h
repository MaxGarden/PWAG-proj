#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <map>
#include <functional>

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
    void DestroyObject(const Object* object);
    
    void VisitObjects(const std::function<bool(const Object& object)>& visitor) const noexcept;
    
    bool IsAnyCollectible() const noexcept;
    
    Shader* EnsureShader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) const noexcept;
    
    Camera& GetMainCamera();

private:
    void InitScene();
    
    void AddFloor();
    void AddSkybox();
    
private:
    std::unique_ptr<Camera> m_camera;
    std::vector<std::unique_ptr<Object>> m_objects;
    
    mutable std::map<std::tuple<std::string, std::string>, std::unique_ptr<Shader>> m_shadersMap;
};

