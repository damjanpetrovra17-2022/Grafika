#pragma once

#include <glm/glm.hpp>

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    AABB()
        : min(0.0f), max(0.0f)
    {
    }

    AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
        : min(minPoint), max(maxPoint)
    {
    }

    bool contains(const glm::vec3& point) const
    {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }

    glm::vec3 clamp(const glm::vec3& point) const
    {
        return glm::vec3(
            glm::clamp(point.x, min.x, max.x),
            glm::clamp(point.y, min.y, max.y),
            glm::clamp(point.z, min.z, max.z)
        );
    }

    glm::vec3 center() const
    {
        return (min + max) * 0.5f;
    }

    glm::vec3 size() const
    {
        return max - min;
    }
};
