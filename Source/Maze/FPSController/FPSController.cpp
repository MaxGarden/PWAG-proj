#include "FPSController.h"
#include <vector>

static const glm::vec3 upDirection =
{
    0.0f,
    1.0f,
    0.0f
};

FPSController::FPSController(SceneManager& sceneManager, glm::vec3 position, float moveSpeed, float lookSpeed) :
    m_initialPosition{position},
    m_moveSpeed{moveSpeed},
    m_lookSpeed{lookSpeed},
    m_sceneManager{sceneManager}
{
    auto& camera = m_sceneManager.GetMainCamera();
    
    camera.SetPosition(m_initialPosition);
    camera.SetRotation({});
}

void FPSController::Update()
{
    const auto currentTime = glfwGetTime();
    const auto deltaTime = static_cast<float>(currentTime - m_lastTime);
    
    Update(deltaTime);
    
    m_lastTime = glfwGetTime();
}

void FPSController::Update(float deltaTime)
{
    HandleInput(deltaTime);
    m_initialized = true;
}

void FPSController::HandleInput(float deltaTime)
{
    if(!m_sceneManager.IsAnyCollectible())
        return;
    
    HandleMouse(deltaTime);
    HandleKeyboard(deltaTime);
}

void FPSController::HandleMouse(float deltaTime)
{
    static const auto epsilon = 0.0001f;
    
    auto& camera = m_sceneManager.GetMainCamera();
    const auto window = camera.GetWindow();
    
    double cursorPositionX, cursorPositionY;
    glfwGetCursorPos(window, &cursorPositionX, &cursorPositionY);
    
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    const auto cursorCenterPositionX = windowWidth / 2.0;
    const auto cursorCenterPositionY = windowHeight / 2.0;
    glfwSetCursorPos(window, cursorCenterPositionX, cursorCenterPositionY);
    
    if(!m_initialized)
        return;
    
    const auto deltaX = cursorPositionX - cursorCenterPositionX;
    const auto deltaY = cursorPositionY - cursorCenterPositionY;
    
    auto cameraRotation = camera.GetRotation();
    if (abs(deltaX) > epsilon)
    {
        cameraRotation.x += m_lookSpeed * deltaX;
        m_isAnyDirectionDirty = true;
    }
    
    if (abs(deltaY) > epsilon)
    {
        cameraRotation.y -= m_lookSpeed * deltaY;
        cameraRotation.y = fmin(glm::radians(89.0f), fmax(glm::radians(-89.0f), cameraRotation.y));
        m_isAnyDirectionDirty = true;
    }
    
    if(cameraRotation != camera.GetRotation())
        camera.SetRotation(cameraRotation);
}

void FPSController::HandleKeyboard(float deltaTime)
{
    static auto canChangeFreeLookMode = true;
    static auto canChangePolygonMode = true;
    
    auto& camera = m_sceneManager.GetMainCamera();
    const auto window = camera.GetWindow();
    const auto& cameraPosition = camera.GetPosition();
    
    const auto addToPosition = [this, &camera, &cameraPosition](const auto& delta)
    {
        auto newPosition = cameraPosition + delta;
        
        auto canMove = true;
        if(!m_freeLook)
        {
            newPosition.y = m_initialPosition.y;
        
            auto newPositionX = newPosition;
            newPositionX.z = cameraPosition.z;
            
            const auto canMoveX = CanMoveOnPosition(newPositionX);
            
            auto newPositionZ = newPosition;
            newPositionZ.x = cameraPosition.x;
            
            const auto canMoveZ = CanMoveOnPosition(newPositionZ);
            
            canMove = canMoveX || canMoveZ;
            newPosition.x = canMoveX ? newPosition.x : cameraPosition.x;
            newPosition.z = canMoveZ ? newPosition.z : cameraPosition.z;
        }
        
        if(canMove)
        {
            m_isAnyDirectionDirty = true;
            camera.SetPosition(newPosition);
            
            if(!m_freeLook)
                GatherCollectibles();
        }
    };
    
    const auto isKeyPressed = [&window](int keyCode)
    {
        return glfwGetKey(window, keyCode) == GLFW_PRESS;
    };
    
    const auto& moveFrontDirection = m_freeLook ? camera.GetFrontDirection() : GetMoveFrontDirection();
    const auto& moveRightDirection = m_freeLook ? GetRightDirection() : GetMoveRightDirection();
    
    if (isKeyPressed(GLFW_KEY_UP) || isKeyPressed(GLFW_KEY_W))
        addToPosition(moveFrontDirection * deltaTime * m_moveSpeed);
    
    if (isKeyPressed(GLFW_KEY_DOWN) || isKeyPressed(GLFW_KEY_S))
        addToPosition(-moveFrontDirection * deltaTime * m_moveSpeed);
    
    if (isKeyPressed(GLFW_KEY_RIGHT) || isKeyPressed(GLFW_KEY_D))
        addToPosition(-moveRightDirection * deltaTime * m_moveSpeed);
    
    if (isKeyPressed(GLFW_KEY_LEFT) || isKeyPressed(GLFW_KEY_A))
        addToPosition(moveRightDirection * deltaTime * m_moveSpeed);
    
    if(isKeyPressed(GLFW_KEY_F))
    {
        if(canChangeFreeLookMode)
        {
            canChangeFreeLookMode = false;
            m_freeLook = !m_freeLook;
            addToPosition(glm::vec3{});
        }
    }
    else
        canChangeFreeLookMode = true;
    
    if(isKeyPressed(GLFW_KEY_G))
    {
        if(canChangePolygonMode)
        {
            canChangePolygonMode = false;
            m_polygonMode = m_polygonMode == GL_FILL ? GL_LINE : GL_FILL;
            glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
        }
    }
    else
        canChangePolygonMode = true;
}

bool FPSController::CanMoveOnPosition(const glm::vec3& position, float radius) const noexcept
{
    auto result = true;
    m_sceneManager.VisitObjects([&position, &result, radius](const auto& object)
    {
        if(!(object.GetFlags() & Object::Colliding))
            return true;
        
        return result = !object.IsCollidingWithSphere(position, radius);
    });
    
    return result;
}

void FPSController::RecalculateDirectionsIfNeeded() const noexcept
{
    if(!m_isAnyDirectionDirty)
        return;
    
    auto& camera = m_sceneManager.GetMainCamera();
    m_rightDirection = glm::normalize(glm::cross(upDirection, camera.GetFrontDirection()));
    
    const auto& cameraRotation = camera.GetRotation();
    m_moveFrontDirection = glm::normalize(glm::vec3
    {
        cos(cameraRotation.x),
        0.0f,
        sin(cameraRotation.x)
    });
    
    m_moveRightDirection = glm::normalize(glm::cross(upDirection, m_moveFrontDirection));
    m_isAnyDirectionDirty = false;
}

const glm::vec3& FPSController::GetRightDirection() const noexcept
{
    RecalculateDirectionsIfNeeded();
    return m_rightDirection;
}

const glm::vec3& FPSController::GetMoveFrontDirection() const noexcept
{
    RecalculateDirectionsIfNeeded();
    return m_moveFrontDirection;
}

const glm::vec3& FPSController::GetMoveRightDirection() const noexcept
{
    RecalculateDirectionsIfNeeded();
    return m_moveRightDirection;
}

void FPSController::GatherCollectibles(float radius) const
{
    const auto& position = m_sceneManager.GetMainCamera().GetPosition();
    std::vector<const Object*> objectsToDestroy;
    
    m_sceneManager.VisitObjects([&objectsToDestroy, &position, radius](const auto& object)
    {
        if(!(object.GetFlags() & Object::Collectable))
            return true;
        
        if(object.IsCollidingWithSphere(position, radius))
            objectsToDestroy.emplace_back(&object);
        
        return true;
    });
    
    for(const auto& object : objectsToDestroy)
        m_sceneManager.DestroyObject(object);
}
