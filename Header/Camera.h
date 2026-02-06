#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "AABB.h"

class Camera
{
public:
    Camera();
    Camera(const glm::vec3& position, float yaw, float pitch);

    void update(float deltaTime);

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix(float aspect) const;

    glm::vec3 getPosition() const { return m_position; }
    void setPosition(const glm::vec3& pos) { m_position = pos; }

    glm::vec3 getFront() const { return m_front; }
    glm::vec3 getRight() const { return m_right; }
    glm::vec3 getUp() const { return m_up; }

    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }

    void setBounds(const AABB& bounds) { m_bounds = bounds; }
    const AABB& getBounds() const { return m_bounds; }
    
    void setAdditionalBounds(const std::vector<AABB>& additionalBounds) { m_additionalBounds = additionalBounds; }

    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
    void setMoveSpeed(float speed) { m_moveSpeed = speed; }
    void setBoundsPadding(float padding) { m_boundsPadding = padding; }
    void setFOV(float fov) { m_fov = fov; }

private:
    void updateVectors();
    void processMouseLook(float deltaX, float deltaY);
    void processMovement(float deltaTime);
    void clampToBounds();

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;

    float m_fov;
    float m_nearPlane;
    float m_farPlane;

    float m_mouseSensitivity;
    float m_moveSpeed;

    AABB m_bounds;
    float m_boundsPadding;
    std::vector<AABB> m_additionalBounds;  // For stairs, platforms, etc.
};
