#pragma once

#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;  // Should be normalized
    
    Ray()
        : origin(0.0f)
        , direction(0.0f, 0.0f, -1.0f)
    {
    }
    
    Ray(const glm::vec3& orig, const glm::vec3& dir)
        : origin(orig)
        , direction(glm::normalize(dir))
    {
    }
    
    glm::vec3 at(float t) const
    {
        return origin + direction * t;
    }
};
