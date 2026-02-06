#include "../Header/Camera.h"
#include "../Header/Input.h"
#include <GLFW/glfw3.h>
#include <algorithm>

Camera::Camera()
    : m_position(0.0f, 1.7f, 5.0f)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_fov(60.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(100.0f)
    , m_mouseSensitivity(0.1f)
    , m_moveSpeed(3.0f)
    , m_bounds(glm::vec3(-10.0f, 0.5f, -10.0f), glm::vec3(10.0f, 5.0f, 10.0f))
    , m_boundsPadding(0.3f)
{
    updateVectors();
}

Camera::Camera(const glm::vec3& position, float yaw, float pitch)
    : m_position(position)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_fov(60.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(100.0f)
    , m_mouseSensitivity(0.1f)
    , m_moveSpeed(3.0f)
    , m_bounds(glm::vec3(-10.0f, 0.5f, -10.0f), glm::vec3(10.0f, 5.0f, 10.0f))
    , m_boundsPadding(0.3f)
{
    updateVectors();
}

void Camera::update(float deltaTime)
{
    // Process mouse look (always enabled - cursor locked at center)
    float mouseX = Input::getMouseDeltaX();
    float mouseY = Input::getMouseDeltaY();
    processMouseLook(mouseX, mouseY);

    // Process keyboard movement
    processMovement(deltaTime);

    // Clamp position to bounds
    clampToBounds();
}

glm::mat4 Camera::viewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::projectionMatrix(float aspect) const
{
    return glm::perspective(glm::radians(m_fov), aspect, m_nearPlane, m_farPlane);
}

void Camera::updateVectors()
{
    // Calculate front vector from yaw and pitch
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // Recalculate right and up vectors
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::processMouseLook(float deltaX, float deltaY)
{
    deltaX *= m_mouseSensitivity;
    deltaY *= m_mouseSensitivity;

    m_yaw += deltaX;
    m_pitch -= deltaY;  // Inverted Y for natural feel

    // Clamp pitch to avoid gimbal lock / flipping
    m_pitch = std::max(-89.0f, std::min(89.0f, m_pitch));

    updateVectors();
}

void Camera::processMovement(float deltaTime)
{
    float velocity = m_moveSpeed * deltaTime;

    // Forward direction on XZ plane (no Y component for ground movement)
    glm::vec3 forwardXZ = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
    glm::vec3 rightXZ = glm::normalize(glm::vec3(m_right.x, 0.0f, m_right.z));

    glm::vec3 desiredPosition = m_position;

    // Arrow keys for movement (forward/back, strafe left/right)
    if (Input::isKeyDown(GLFW_KEY_UP) || Input::isKeyDown(GLFW_KEY_W))
    {
        desiredPosition += forwardXZ * velocity;
    }
    if (Input::isKeyDown(GLFW_KEY_DOWN) || Input::isKeyDown(GLFW_KEY_S))
    {
        desiredPosition -= forwardXZ * velocity;
    }
    if (Input::isKeyDown(GLFW_KEY_LEFT) || Input::isKeyDown(GLFW_KEY_A))
    {
        desiredPosition -= rightXZ * velocity;
    }
    if (Input::isKeyDown(GLFW_KEY_RIGHT) || Input::isKeyDown(GLFW_KEY_D))
    {
        desiredPosition += rightXZ * velocity;
    }
    
    // Collision resolution
    const float cameraRadius = 0.30f;
    const float epsilon = 0.001f;
    const float maxStepHeight = 0.4f;  // Maximum height we can step up
    
    // Find platform/stair we're standing on
    float supportY = m_position.y;  // Default: stay at current height
    bool foundSupport = false;
    
    for (const AABB& bound : m_additionalBounds)
    {
        // Check if XZ position is over this platform/stair
        if (desiredPosition.x >= bound.min.x - cameraRadius && 
            desiredPosition.x <= bound.max.x + cameraRadius &&
            desiredPosition.z >= bound.min.z - cameraRadius && 
            desiredPosition.z <= bound.max.z + cameraRadius)
        {
            float platformTop = bound.max.y;
            
            // If we're close to standing on this platform
            if (m_position.y >= platformTop - maxStepHeight && 
                m_position.y <= platformTop + epsilon)
            {
                supportY = std::max(supportY, platformTop);
                foundSupport = true;
            }
        }
    }
    
    // Check horizontal collision (prevent passing through walls)
    glm::vec3 resolvedPosition = desiredPosition;
    resolvedPosition.y = supportY;  // Use support height
    
    for (const AABB& bound : m_additionalBounds)
    {
        // Only check horizontal collision if we're at a similar height
        if (resolvedPosition.y > bound.max.y + cameraRadius || 
            resolvedPosition.y < bound.min.y - cameraRadius)
            continue;  // Too high or too low to collide
        
        // Check XZ collision
        glm::vec3 closestXZ;
        closestXZ.x = glm::clamp(resolvedPosition.x, bound.min.x, bound.max.x);
        closestXZ.z = glm::clamp(resolvedPosition.z, bound.min.z, bound.max.z);
        
        float dx = resolvedPosition.x - closestXZ.x;
        float dz = resolvedPosition.z - closestXZ.z;
        float distSq = dx * dx + dz * dz;
        
        if (distSq < cameraRadius * cameraRadius && distSq > epsilon)
        {
            // Collision - push out horizontally
            float dist = std::sqrt(distSq);
            float penetration = cameraRadius - dist;
            glm::vec2 pushDir = glm::normalize(glm::vec2(dx, dz));
            
            resolvedPosition.x += pushDir.x * penetration;
            resolvedPosition.z += pushDir.y * penetration;
        }
    }
    
    m_position = resolvedPosition;
}






void Camera::clampToBounds()
{
    // Apply padding to bounds
    AABB paddedBounds(
        m_bounds.min + glm::vec3(m_boundsPadding),
        m_bounds.max - glm::vec3(m_boundsPadding)
    );

    m_position = paddedBounds.clamp(m_position);
}
